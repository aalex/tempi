 /*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "tempi/midi/midiinput.h"
#include <cstdlib>
#include <iostream>
#include <stk/RtMidi.h>

namespace tempi
{

namespace midi
{

static const unsigned char MIDINOTEOFF =       0x80; // channel, pitch, velocity
static const unsigned char MIDINOTEON =        0x90; // channel, pitch, velocity
static const unsigned char MIDICONTROLCHANGE = 0xb0; // channel, controller, value
static const unsigned char MIDIPROGRAMCHANGE = 0xc0; // channel, value
static const unsigned char MIDIPITCHBEND =     0xe0; // channel, value
static const unsigned char MIDI_NOT_SUPPORTED = 0x00;

static unsigned char getMidiEventType(const unsigned char first_byte)
{
    unsigned char type_code;
    if (first_byte >= 0xf0)
        type_code = first_byte;
    else
        type_code = first_byte & 0xf0;
    unsigned char ret;
    switch (type_code)
    {
        case MIDINOTEOFF:
        case MIDINOTEON:
        case MIDICONTROLCHANGE:
        case MIDIPROGRAMCHANGE:
        case MIDIPITCHBEND:
            ret = type_code;
            break;
        default:
            ret = MIDI_NOT_SUPPORTED;
            break;
    }
    return ret;
}

bool MidiInput::isOpen() const
{
    return opened_;
}

/**
 * Callback for incoming MIDI messages.  Called in its thread.
 *
 * We try to get the first MidiRule found for the given MIDI event.
 * (given its pitch, and if on or off)
 *  - NOTE_OFF_RULE: [s:action s:args]
 *  - NOTE_ON_RULE: [s:action s:args]
 *  - CONTROL_MAP_RULE [s:action i:value]
 *  - CONTROL_ON_RULE [s:action s:args]
 *  - CONTROL_OFF_RULE [s:action s:args]
 *  - MIDIPROGRAMCHANGE [s:action i:number]
 */
void MidiInput::input_message_cb(double /* delta_time */, std::vector<unsigned char> *message, void *user_data)
{
    MidiInput* context = static_cast<MidiInput*>(user_data);
    if (context->verbose_)
    {
        std::cout << "MIDI message: (";
        for (unsigned int i = 0; i < message->size(); i++)
            std::cout << (int)message->at(i) << " ";
        std::cout << ")" << std::endl;
    }

    Message result;
    std::vector<unsigned char>::iterator iter;
    for (iter = message->begin(); iter != message->end(); ++iter)
        result.appendUnsignedChar(*iter);
    context->pushMessage(result);

    // if (message->size() <= 1)
    //     return; // Don't support messages with only one byte or less.
    // unsigned char midi_event_type = getMidiEventType(message->at(0));
    // Message result;
    // result.appendUnsignedChar(midi_event_type);
    // switch (midi_event_type)
    // {
    //     case MIDINOTEON:
    //     {
    //         if (context->verbose_)
    //             std::cout << "MIDINOTEON" << std::endl;
    //         if (message->size() < 3)
    //         {
    //             std::cerr << "Note on message should have 4 params" << std::endl;
    //             return;
    //         }
    //         int note_pitch = int(message->at(1));
    //         int velocity = int(message->at(2));
    //         result.appendInt(note_pitch);
    //         result.appendInt(velocity);
    //         break;
    //     }
    //     case MIDINOTEOFF:
    //     {
    //         if (context->verbose_)
    //             std::cout << "MIDINOTEOFF" << std::endl;
    //         int note_pitch = int(message->at(1));
    //         result.appendInt(note_pitch);
    //         //result.appendInt(0); // velocity
    //         break;
    //     }
    //     case MIDICONTROLCHANGE:
    //     {
    //         int controller_number = int(message->at(1));
    //         int control_value = int(message->at(2));
    //         result.appendInt(controller_number);
    //         result.appendInt(control_value);
    //         break;
    //     }
    //     case MIDIPROGRAMCHANGE:
    //     {
    //         int program_number = int(message->at(0) & 0x0f);
    //         result.appendInt(program_number);
    //         break;
    //     }
    //     case MIDIPITCHBEND:
    //     {
    //         int val(message->at(2));
    //         result.appendInt(val);
    //         break;
    //     }
    //     default:
    //         return;
    //         break;
    // }
    // context->pushMessage(result);
}

void MidiInput::enumerateDevices() const
{
    // List inputs.
    std::cout << std::endl << "MIDI input devices: " << ports_count_ << " found." << std::endl;
    std::string portName;
    for (unsigned int i=0; i < ports_count_; i++)
    {
        try
        {
            portName = midi_in_->getPortName(i);
        }
        catch (RtError &name_error)
        {
            name_error.printMessage();
        }
        std::cout << " * " << i << " : " << portName << '\n';
    }
}

void MidiInput::pushMessage(const Message &message)
{
    if (verbose_)
        std::cout << __FUNCTION__ << "(" << message << ")" << std::endl;
    messaging_queue_.push(message);
}

std::vector<Message> MidiInput::poll()
{
    std::vector<Message> ret;
    bool success = true;
    while (success)
    {
        Message message;
        success = messaging_queue_.try_pop(message);
        if (success)
        {
            if (verbose_)
                std::cout << __FUNCTION__ << ": pop " << message << std::endl;
            ret.push_back(message);
        }
    }
    return ret;
}

MidiInput::MidiInput() :
        verbose_(false),
        opened_(false),
        port_(0),
        ports_count_(0),
        client_name_("tempi.midi.in"),
        messaging_queue_()
{
    // RtMidiIn constructor
    // TODO:allow to change client name using a property
    try
    {
        midi_in_ = new RtMidiIn(client_name_);
    }
    catch (RtError &error)
    {
        error.printMessage();
        exit(EXIT_FAILURE); // FIXME: should not exit!!!
    }
    // Check available ports vs. specified.
    ports_count_ = midi_in_->getPortCount();
}

MidiInput::~MidiInput()
{
    delete midi_in_;
}

bool MidiInput::open(unsigned int port)
{
    ports_count_ = midi_in_->getPortCount();
    if (port >= ports_count_)
    {
        //TODO: raise excepion !
        // delete midi_in_;
        std::cout << "Invalid input MIDI port!" << std::endl;
        // usage();
        opened_ = false;
        return false;
    }
    try
    {
        midi_in_->openPort(port);
    }
    catch (RtError &error)
    {
        std::cout << "Error opening MIDI port " << port << std::endl;
        error.printMessage();
        opened_ = false;
        return false;
    }
 
    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue instead of sent to the callback function.
    // TODO:2010-10-03:aalex:Pass only a pointer to the concurrent queue, no the whole MidiInput instance,
    // which is not thread safe
    midi_in_->setCallback(&input_message_cb, (void *) this);
    // Don't ignore sysex, timing, or active sensing messages.
    //midi_in_->ignoreTypes(false, false, false);
    // ignore sysex, timing, or active sensing messages.
    midi_in_->ignoreTypes(true, true, true);
    opened_ = true;
    return true;
}

void MidiInput::setVerbose(bool verbose)
{
    verbose_ = verbose;
}

} // end of namespace

} // end of namespace


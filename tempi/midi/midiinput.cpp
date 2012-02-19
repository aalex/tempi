 /*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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

namespace tempi {
namespace midi {

bool MidiInput::isOpen() const
{
    return opened_;
}

/**
 * Callback for incoming MIDI messages.  Called in its thread.
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
    // TODO:allow to change client name using a attribute
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
    if (midi_in_)
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


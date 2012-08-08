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
#include "tempi/midi_input.h"
#include <cstdlib>
#include <iostream>

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

void MidiInput::process_midi(PtTimestamp timestamp, void *user_data)
{
    std::cout << "coucou" << std::endl;
    MidiInput* context = static_cast<MidiInput*>(user_data);
    
    PmError result;
    PmEvent buffer; /* just one message at a time */
    
    /* do nothing until initialization completes */
    if (! context->portmidi_initialized_)
    {
        std::cout << "exit flag" << std::endl; 
        /*   this flag signals that no more midi processing will be done */
        context->process_midi_exit_flag_ = true;
        return;
    }
    
    /* see if there is any midi input to process */
    if (! context->app_sysex_in_progress_)
    {
        do
        {
            result = Pm_Poll(context->midi_in_);
            if (result)
            {
                int status;
                PmError rslt = (PmError)Pm_Read(context->midi_in_, &buffer, 1);
                if (rslt == pmBufferOverflow) 
                    continue;
                //assert(rslt == 1);
    
                // /* record timestamp of most recent data */
                // last_timestamp = current_timestamp;

                /* the data might be the end of a sysex message that
                   has timed out, in which case we must ignore it.
                   It's a continuation of a sysex message if status
                   is actually a data byte (high-order bit is zero). */
                status = Pm_MessageStatus(buffer.message);
                if (((status & 0x80) == 0) && !context->thru_sysex_in_progress_)
                {
                    continue; /* ignore this data */
                }

                /* sysex processing */
                if (status == MIDI_SYSEX)
                    context->thru_sysex_in_progress_ = true;
                else if ((status & 0xF8) != 0xF8)
                {
                    /* not MIDI_SYSEX and not real-time, so */
                    context->thru_sysex_in_progress_ = false;
                }
                if (context->thru_sysex_in_progress_ && /* look for EOX */
                    (((buffer.message & 0xFF) == MIDI_EOX) ||
                    (((buffer.message >> 8) & 0xFF) == MIDI_EOX) ||
                    (((buffer.message >> 16) & 0xFF) == MIDI_EOX) ||
                    (((buffer.message >> 24) & 0xFF) == MIDI_EOX)))
                {
                    context->thru_sysex_in_progress_ = false;
                }
            }
        } while (result);
    }
}

void MidiInput::enumerateDevices() const
{
    /* list device information */
    int i;
    for (i = 0; i < Pm_CountDevices(); i++)
    {
        const PmDeviceInfo *listinfo = Pm_GetDeviceInfo(i);
        std::cout << i << ": " << listinfo->interf << ", " << listinfo->name ;
        if (listinfo->input)
            std::cout << " (input)" << std::endl;
        if (listinfo->output)
            std::cout << " (output)" << std::endl;
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
    //     client_name_("tempi.midi.in"),
    messaging_queue_(),
    portmidi_initialized_(false),
    app_sysex_in_progress_(false),
    thru_sysex_in_progress_(false)
{
    std::cout << " midi in construct" << std::endl;
    /* starting the timer before starting midi */
    Pt_Start(100, &process_midi, this); /* start a timer with millisecond accuracy */
    /* the timer will call our function, process_midi() every millisecond */
    Pm_Initialize();
    ports_count_ = Pm_CountDevices();
}

MidiInput::~MidiInput()
{
    std::cout << "terminate midi input" << std::endl;

    /* the timer thread could be in the middle of accessing PortMidi stuff */
    /* to detect that it is done, we first clear process_midi_exit_flag and
     * then wait for the timer thread to set it
    */
    process_midi_exit_flag_ = false;
    portmidi_initialized_ = false;

    /* busy wait for flag from timer thread that it is done */
    while (! process_midi_exit_flag_) 
    {
        sleep(1); // std::cout << "busy wait" << std::endl;
    }
    /* at this point, midi thread is inactive and we need to shut down
     * the midi input and output
     */
    
    Pt_Stop(); /* stop the timer */
    //Pm_Close(midi_in_);
    //Pm_Terminate();    
}

unsigned int MidiInput::getDefaultInputDevice()
{
    return Pm_GetDefaultInputDeviceID();
}

bool MidiInput::open()
{
    open(Pm_GetDefaultInputDeviceID());
}

bool MidiInput::open(unsigned int port)
{
    port = 3;
    ports_count_ = Pm_CountDevices();

    const PmDeviceInfo *info = Pm_GetDeviceInfo(port);
    if (info == NULL)
    {
        std::cout << "Could not open input device " << port << std::endl;
        opened_ = false;
    }

    Pm_OpenInput(&midi_in_, 
         port, 
         NULL /* driver info */,
         0 /* use default input size */,
         NULL, /* use PortTime */
         NULL /* time info */);
    /* Note: if you set a filter here, then this will filter what goes
 t  o the MIDI THRU port. You may not want to do this.
    */
    Pm_SetFilter(midi_in_, PM_FILT_ACTIVE | PM_FILT_CLOCK);

    portmidi_initialized_ = true; /* enable processing in the midi thread -- yes, this
                     is a shared variable without synchronization, but
                     this simple assignment is safe */
    opened_ = true;
    process_midi(0, this);
    return true;
}

void MidiInput::setVerbose(bool verbose)
{
    verbose_ = verbose;
}

} // end of namespace
} // end of namespace


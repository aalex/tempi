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

/**
 * @file
 * The MidiInput class
 */

#ifndef __TEMPI_MIDI_MIDIINPUT_H__
#define __TEMPI_MIDI_MIDIINPUT_H__

#include "tempi/concurrentqueue.h"
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include <stk/RtMidi.h>

namespace tempi
{

namespace midi
{

/** 
 * MIDI input using RtMidi.
 */
class MidiInput
{
    public:
        typedef std::tr1::shared_ptr<MidiInput> ptr;
        MidiInput();
        ~MidiInput();
        /** Opens a MIDI source device. */
        bool open(unsigned int port);
        unsigned int getPort() const;
        /** Prints the list of MIDI source devices. */
        void enumerateDevices() const;
        // TODO: std::vector<boost::tuple<unsigned int, std::string> > enumerateDevices();
        bool isOpen() const;
        bool isVerbose() const;
        /** Sets it verbose or not. */
        void setVerbose(bool verbose);
        /** Flushes the messages from the asynchronous messaging queue. */
        std::vector<Message> poll();
    private:
        bool verbose_;
        bool opened_;
        unsigned int port_;
        unsigned int ports_count_;
        std::string client_name_;
        ConcurrentQueue<Message> messaging_queue_;
        RtMidiIn *midi_in_;
        // methods:
        void pushMessage(const Message &message);
        static void input_message_cb(double delta_time,
            std::vector<unsigned char> *message, void *user_data);
};

} // end of namespace

} // end of namespace

#endif // include guard


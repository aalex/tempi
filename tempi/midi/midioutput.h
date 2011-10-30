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
 * The MidiOutput class
 */

#ifndef __TEMPI_MIDI_MIDIOUTPUT_H__
#define __TEMPI_MIDI_MIDIOUTPUT_H__

#include "tempi/concurrentqueue.h"
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include <stk/RtMidi.h>

namespace tempi
{

namespace midi
{

/** 
 * MIDI output using RtMidi.
 */
class MidiOutput
{
    public:
        typedef std::tr1::shared_ptr<MidiOutput> ptr;
        MidiOutput();
        ~MidiOutput();
        /** Opens a MIDI source device. */
        bool open(unsigned int port);
        unsigned int getPort() const;
        /** Prints the list of MIDI sink devices. */
        void enumerateDevices();
        // TODO: std::vector<boost::tuple<unsigned int, std::string> > enumerateDevices();
        bool isOpen() const;
        bool isVerbose() const;
        /** Sets it verbose or not. */
        void setVerbose(bool verbose);
        bool sendMessage(const Message &message) const;
    private:
        bool verbose_;
        bool opened_;
        unsigned int port_;
        unsigned int ports_count_;
        std::string client_name_;
        //ConcurrentQueue<Message> messaging_queue_;
        RtMidiOut *midi_out_;
};

} // end of namespace

} // end of namespace

#endif // include guard


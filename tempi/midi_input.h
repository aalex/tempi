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

/**
 * @file
 * The MidiInput class
 */

#ifndef __TEMPI_MIDI_MIDIINPUT_H__
#define __TEMPI_MIDI_MIDIINPUT_H__

#ifndef MIDI_SYSEX
#define MIDI_SYSEX 0xf0
#endif
#ifndef MIDI_EOX
#define MIDI_EOX 0xf7
#endif

#include "tempi/concurrentqueue.h"
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include <portmidi.h>
#include <porttime.h>

namespace tempi {
namespace midi {

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
	bool open(); //default input device
	unsigned int getDefaultInputDevice();
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
	//        std::string client_name_;
        ConcurrentQueue<Message> messaging_queue_;
	//the midi in
	PmStream *midi_in_;
	/* process_midi_exit_flag is set when the timer thread shuts down */
	bool process_midi_exit_flag_;
	bool portmidi_initialized_;
	bool app_sysex_in_progress_;
	bool thru_sysex_in_progress_;
	// methods:
        void pushMessage(const Message &message);
	static void process_midi(PtTimestamp timestamp, void *userData);
        static void input_message_cb(double delta_time,
				     std::vector<unsigned char> *message, 
				     void *user_data);
};

} // end of namespace
} // end of namespace

#endif // include guard


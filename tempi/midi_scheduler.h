/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * The Midi class
 */

#ifndef __TEMPI_MIDI_MIDI_SCHEDULER_H__
#define __TEMPI_MIDI_MIDI_SCHEDULER_H__

#include "tempi/sharedptr.h"
#include <portmidi.h>
#include <porttime.h>
#include <map>
#include <queue>

#ifndef MIDI_SYSEX
#define MIDI_SYSEX 0xf0
#endif
#ifndef MIDI_EOX
#define MIDI_EOX 0xf7
#endif

namespace tempi {
namespace midi {

/** 
 * MIDI scheduler for midi use based on portmidi. This class must be used as a singleton 
 * This is done with the midi (device) class.
 */
class MidiScheduler
{
    public:
        typedef std::tr1::shared_ptr<MidiScheduler> ptr;
        MidiScheduler();
        ~MidiScheduler();
        // TODO: std::vector<boost::tuple<unsigned int, std::string> > enumerateDevices();
        PmStream *add_input_stream(int id);
        bool remove_input_stream(PmStream *stream);
        PmEvent poll(PmStream *stream);
        bool is_queue_empty(PmStream *stream);

        PmStream *add_output_stream(int id);
        bool remove_output_stream(PmStream *stream);
        bool push_message(PmStream *stream, unsigned char status, unsigned char data1, unsigned char data2);

    private:
        //vector containing the streams to process 
        std::map<PmStream *,std::queue<PmEvent> *> input_queues_;
        std::map<PmStream *,std::queue<PmEvent> *> output_queues_;

        /* process_midi_exit_flag is set when the timer thread shuts down */
        bool process_midi_exit_flag_;
        bool portmidi_initialized_;
        bool app_sysex_in_progress_;
        bool thru_sysex_in_progress_;
        static void process_midi(PtTimestamp timestamp, void *userData);
};
 
} // end of namespace
} // end of namespace

#endif // include guard


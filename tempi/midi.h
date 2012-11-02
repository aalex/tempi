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

#ifndef __TEMPI_MIDI_MIDI_H__
#define __TEMPI_MIDI_MIDI_H__

#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include "tempi/midi_scheduler.h"
#include <map>
#include <vector>

namespace tempi {
namespace midi {

/** 
 * MIDI device using portmidi.
 */
class Midi
{
    public:
        typedef std::tr1::shared_ptr<Midi> ptr;
        typedef enum {
            SOURCE = 1,
            DESTINATION = 2
            // BOTH = 4
        } DeviceDirection;
        Midi();
        ~Midi();

        /** Prints the list of MIDI devices. */
        void enumerate_devices() const;
        /** Returns the list of desired MIDI devices. */
        static std::map<int, std::string> listDevices(DeviceDirection direction);
        
        //input
        int get_default_input_device_id();
        bool open_input_device(int id);
        void close_input_device(int id);
        bool is_queue_empty(int id);
        std::vector<unsigned char> poll(int id);

        //ouput
        int get_default_output_device_id();
        bool open_output_device(int id);
        void close_output_device(int id);
        bool send_message_to_output(int id, unsigned char status, unsigned char data1, unsigned char data2);

        bool is_open(int id);

    private:
        static MidiScheduler *sched_;
        static int streams;
        //keeping track of device openned by the current instance
        std::map<int, PmStream *> openned_streams_;
};

} // end of namespace
} // end of namespace

#endif // include guard


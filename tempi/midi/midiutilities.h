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
 * Utilities functions to parse MIDI events.
 */
#ifndef __TEMPI_MIDI_MIDIUTILITIES_H__
#define __TEMPI_MIDI_MIDIUTILITIES_H__

namespace tempi {
namespace midi {
namespace utilities {

const unsigned char MIDINOTEOFF =       0x80; // channel, pitch, velocity
const unsigned char MIDINOTEON =        0x90; // channel, pitch, velocity
const unsigned char MIDICONTROLCHANGE = 0xb0; // channel, controller, value
const unsigned char MIDIPROGRAMCHANGE = 0xc0; // channel, value
const unsigned char MIDIPITCHBEND =     0xe0; // channel, value
const unsigned char MIDI_NOT_SUPPORTED = 0x00;

unsigned char getMidiEventType(const unsigned char first_byte);
unsigned char getChannelNumber(unsigned char firstByte);

} // end of namespace
} // end of namespace
} // end of namespace

#endif // ifndef


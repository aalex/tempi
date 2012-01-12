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

#include "tempi/midi/midiutilities.h"

namespace tempi {
namespace midi {
namespace utilities {

unsigned char getMidiEventType(const unsigned char first_byte)
{
    unsigned char type_code;
    if (first_byte >= 0xf0)
        type_code = first_byte; // Some type codes use two bytes?
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
            ret = MIDI_NOT_SUPPORTED; // FIXME
            break;
    }
    return ret;
}

unsigned char getChannelNumber(unsigned char firstByte)
{
    return (firstByte & 0x0F);
}

} // end of namespace
} // end of namespace
} // end of namespace


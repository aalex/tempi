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

#include "tempi/midi/midiroutenode.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace midi {

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
        type_code = first_byte; // Soem type codes use two bytes?
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

static unsigned char getChannelNumber(unsigned char c)
{
    return (c & 0x0F);
}

MidiRouteNode::MidiRouteNode() :
    Node()
{
    addOutlet();
//    addAttribute("types", Message("s", "note"), "List of strings for MIDI event types. "
//        "Valid values are \"note\" and \"control\".", false);
}

void MidiRouteNode::processMessage(unsigned int inlet, const Message &message)
{
    if (! message.indexMatchesType(0, 'C'))
    {
        std::cerr << "MidiRouteNode::" << __FUNCTION__ << ": First atom should be an unsigned char: " << message << std::endl;
        return;
    }
    unsigned char midi_event_type = getMidiEventType(message.getUnsignedChar(0));
    Message result;
    switch (midi_event_type)
    {
        case MIDI_NOT_SUPPORTED:
        {
            std::cerr << "Unsupported MIDI event type: " << message << std::endl;
            return;
            break;
        }
        case MIDINOTEON:
        case MIDINOTEOFF:
        {
            if (! message.typesMatch("CCC"))
            {
                std::cerr << "Note on/off messages should have 3 atoms:" << message << std::endl;
                return;
            }
            int channel_number = int(getChannelNumber(message.getUnsignedChar(0)));
            int note_pitch = int(message.getUnsignedChar(1));
            int velocity = int(message.getUnsignedChar(2));

            result.appendString("note");
            result.appendInt(channel_number);
            result.appendInt(note_pitch);
            result.appendInt(velocity);
            break;
        }
        case MIDICONTROLCHANGE:
        {
            if (! message.typesMatch("CCC"))
            {
                std::cerr << "Control change messages should have 3 atoms:" << message << std::endl;
                return;
            }
            int channel_number = int(getChannelNumber(message.getUnsignedChar(0)));
            int controller_number = int(message.getUnsignedChar(1));
            int control_value = int(message.getUnsignedChar(2));

            result.appendString("control");
            result.appendInt(channel_number);
            result.appendInt(controller_number);
            result.appendInt(control_value);
            break;
        }
        case MIDIPROGRAMCHANGE:
        {
            std::cerr << "Program change messages are not supported." << std::endl;
            return;
        }
        case MIDIPITCHBEND:
        {
            if (! message.typesMatch("CCC"))
            {
                std::cerr << "Program change messages should have 3 atoms: " << message << std::endl;
                return;
            }
            int channel_number = int(getChannelNumber(message.getUnsignedChar(0)));
            int bend = int(message.getUnsignedChar(2));

            result.appendString("pitch_bend");
            result.appendInt(channel_number);
            result.appendInt(bend);
            break;
        }
    }
    output(0, result);
}

// void MidiRouteNode::onAttributeChanged(const char *name, const Message &value)
// {
//     using utils::stringsMatch;
//     if (stringsMatch(name, "types"))
//     {
//         for (unsigned int i = 0; i < value.getSize(); ++i)
//         {
//             if (value.indexMatchesType(i, 's'))
//             {
//                 // TODO
//             }
//         }
//     }
// }

} // end of namespace
} // end of namespace


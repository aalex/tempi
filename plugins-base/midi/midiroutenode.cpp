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

#include "plugins-base/midi/midiroutenode.h"
#include "plugins-base/midi/midiutilities.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

MidiRouteNode::MidiRouteNode() :
    Node()
{
    setShortDocumentation("Routes MIDI messages according to their type.");
    addOutlet("0", "Prepends MIDI messages (converted to integers) by their type name. (\"note\", \"control\" or \"pitch_bend\")");
    //Outputs MIDI messages of a single event type.");
    addInlet("0", "You must send MIDI messages to this inlet. (list of unsigned characters)");
//    addAttribute(Attribute::ptr("types", Message("s", "note"), "List of strings for MIDI event types. "
//        "Valid values are \"note\" and \"control\".", false));
}

void MidiRouteNode::processMessage(const char *inlet, const Message &message)
{
    using namespace midi_utilities;
    if (! message.indexMatchesType(0, 'C'))
    {
        std::cerr << "MidiRouteNode::" << __FUNCTION__ << ": First atom should be an unsigned char: " << message << std::endl;
        return;
    }
    unsigned char midi_event_type = getMidiEventType(message.getUnsignedChar(0));
    Message result;
    switch (midi_event_type)
    {
        case midi_utilities::MIDI_NOT_SUPPORTED:
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
    output("0", result);
}

// void MidiRouteNode::onNodeAttibuteChanged(const char *name, const Message &value)
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


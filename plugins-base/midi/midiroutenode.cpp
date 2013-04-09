/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
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
#include "tempi/log.h"
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
    if (! message.indexMatchesType(0, 'i'))
    {
        std::ostringstream os;
        os << "MidiRouteNode::" << __FUNCTION__ << ": First atom should be an int: " << message;
        Logger::log(ERROR, os);
        return;
    }
    unsigned char midi_event_type = getMidiEventType(message.getInt(0));
    Message result;
    switch (midi_event_type)
    {
        case midi_utilities::MIDI_NOT_SUPPORTED:
        {
            std::ostringstream os;
            os << "Unsupported MIDI event type: " << message;
            Logger::log(ERROR, os);
            return;
            break;
        }
        case MIDINOTEON:
        case MIDINOTEOFF:
        {
            if (! message.typesMatch("iii"))
            {
                std::ostringstream os;
                os << "Note on/off messages should have 3 atoms:" << message;
                Logger::log(ERROR, os);
                return;
            }
            Int channel_number = getChannelNumber(message.getInt(0));
            Int note_pitch = message.getInt(1);
            Int velocity = message.getInt(2);

            result.appendString("note");
            result.appendInt(channel_number);
            result.appendInt(note_pitch);
            result.appendInt(velocity);
            break;
        }
        case MIDICONTROLCHANGE:
        {
            if (! message.typesMatch("iii"))
            {
                std::ostringstream os;
                os << "Control change messages should have 3 atoms:" << message;
                Logger::log(ERROR, os);
                return;
            }
            Int channel_number = getChannelNumber(message.getInt(0));
            Int controller_number = message.getInt(1);
            Int control_value = message.getInt(2);

            result.appendString("control");
            result.appendInt(channel_number);
            result.appendInt(controller_number);
            result.appendInt(control_value);
            break;
        }
        case MIDIPROGRAMCHANGE:
        {
            // TODO
            std::ostringstream os;
            os << "MidiRouteNode: Program change messages are not supported.";
            Logger::log(ERROR, os);
            return;
        }
        case MIDIPITCHBEND:
        {
            if (! message.typesMatch("iii"))
            {
                std::ostringstream os;
                os << "MidiRouteNode: Pitch bend messages should have 3 atoms: " << message;
                Logger::log(ERROR, os);
                return;
            }
            Int channel_number = getChannelNumber(message.getInt(0));
            Int bend = message.getInt(2);

            result.appendString("pitch_bend");
            result.appendInt(channel_number);
            result.appendInt(bend);
            break;
        }
    }
    output("0", result);
}

// void MidiRouteNode::onNodeAttributeChanged(const char *name, const Message &value)
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


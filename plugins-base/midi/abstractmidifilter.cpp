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

#include "plugins-base/midi/abstractmidifilter.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi{
namespace plugins_base {

const char * const AbstractMidiEventBuilderNode::INTS_INLET = "ints";
const char * const AbstractMidiEventBuilderNode::EVENT_OUTLET = "midi_event";
const char * const AbstractMidiEventBuilderNode::CHANNEL_ATTR = "channel";

AbstractMidiEventBuilderNode::AbstractMidiEventBuilderNode() :
    Node()
{
    addInlet(INTS_INLET, "Incoming list of integers");
    addOutlet(EVENT_OUTLET, "Resulting MIDI messages (unsigned characters) flow through this outlet.");
    addAttribute(Attribute::ptr(new Attribute(CHANNEL_ATTR, Message("i", 1), "MIDI channel (from 1 to 16)")));
}

bool AbstractMidiEventBuilderNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, CHANNEL_ATTR))
    {
        int channel = value.getInt(0);
        if ((channel < 1) || (channel > 16 ))
            return false;
    }
    return true;
}

void AbstractMidiEventBuilderNode::processMessage(const char *inlet, const Message &message)
{
    {
        std::ostringstream os;
        os << "AbstractMidiEventBuilderNode." << __FUNCTION__ << ": " << message;
        Logger::log(INFO, os);
    }
    if (! utils::stringsMatch(INTS_INLET, inlet))
        return;
    std::vector<int> ints;
    for (unsigned int i = 0; i < message.getSize(); ++i)
    {
        if (message.indexMatchesType(i, INT)) 
            ints.push_back(message.getInt(i));
        else if (message.indexMatchesType(i, FLOAT))
            ints.push_back((int) message.getFloat(i));
        else 
        {
            std::ostringstream os;
            os << "Invalid message type in AbstractMidiEventBuilderNode: " << message;
            Logger::log(ERROR, os);
            return;
        }
    }
    Message result;
    if (this->buildMidiEvent(ints, result))
        output(EVENT_OUTLET, result);
    else
    {
        std::ostringstream os;
        os << "Could not build MIDI event from " << message;
        Logger::log(ERROR, os);
        return;
    }
}


} // end of namespace
} // end of namespace


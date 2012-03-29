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

#include "plugins-base/midi/midisendernode.h"
#include "tempi/log.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

const char * const MidiSenderNode::EVENTS_INLET = "0"; // TODO: rename to events?
const char * const MidiSenderNode::PORT_ATTR = "port";

MidiSenderNode::MidiSenderNode() :
    Node()
{
    this->setShortDocumentation("Sends MIDI messages to a single device.");
    this->addInlet(EVENTS_INLET, "Send MIDI received from this inlet. (list of unsigned characters)");
    this->addAttribute(Attribute::ptr(new Attribute(PORT_ATTR, Message("i", 0), "STK MIDI device index.")));
    midi_output_.reset(new midi::MidiOutput);
}

void MidiSenderNode::onInit()
{
    this->open(0);
}

bool MidiSenderNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, PORT_ATTR))
    {
        bool success = open((unsigned int) value.getInt(0));
        // TODO: if (! success) { this->setAttribute(PORT_ATTR, Message("i", 0))); return false; }
        return success;
    }
    else
        return true;
}

bool MidiSenderNode::open(unsigned int port)
{
    {
        std::ostringstream os;
        os << "MidiSenderNode: open port " << port;
        Logger::log(INFO, os);
    }
    midi_output_.reset(new midi::MidiOutput);
    return midi_output_->open(port);
}

// TODO: output the list of devices upon query

void MidiSenderNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, EVENTS_INLET))
        return;
    if (midi_output_->isOpen())
    {
        bool ok = midi_output_->sendMessage(message);
        if (! ok)
        {
            std::ostringstream os;
            os << "MidiSenderNode::" << __FUNCTION__ << ": Error sending " << message;
            Logger::log(WARNING, os);
        }
    }
    else
    {
        std::ostringstream os;
        os << "MidiSenderNode::" << __FUNCTION__ << ": MIDI output is not open. Cannot send " << message;
        Logger::log(WARNING, os);
    }
}

} // end of namespace
} // end of namespace


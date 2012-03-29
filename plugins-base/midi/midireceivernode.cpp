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

#include "plugins-base/midi/midireceivernode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

const char * const MidiReceiverNode::EVENTS_OUTLET = "0";
const char * const MidiReceiverNode::PORT_ATTR = "port";

MidiReceiverNode::MidiReceiverNode() :
    Node()
{
    this->setShortDocumentation("Receives MIDI messages from a single device.");
    this->addOutlet(EVENTS_OUTLET, "MIDI messages (unsigned characters) flow through this outlet.");
    this->addAttribute(Attribute::ptr(new Attribute(PORT_ATTR, Message("i", 0), "STK MIDI device index.")));
    midi_input_.reset(new midi::MidiInput);
}

bool MidiReceiverNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    //std::cout << "MidiReceiverNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    if (utils::stringsMatch(name, PORT_ATTR))
    {
        bool success = open((unsigned int) value.getInt(0));
        // TODO: if (! success) { this->setAttribute(PORT_ATTR, Message("i", 0))); return false; }
        return success;
    }
    else
        return true;
}

bool MidiReceiverNode::open(unsigned int port)
{
    {
        std::ostringstream os;
        os << "MidiReceiverNode: open port " << port;
        Logger::log(INFO, os);
    }
    midi_input_.reset(new midi::MidiInput);
    return midi_input_->open(port);
}

void MidiReceiverNode::onInit()
{
    this->open(0);
}

void MidiReceiverNode::doTick()
{
    if (! midi_input_->isOpen())
    {
        std::cerr << "MidiReceiverNode::" << __FUNCTION__ << "(): MidiInput is not initialized. Please specifiy a port number." << std::endl;
        return;
    }
    std::vector<Message> messages = midi_input_->poll();
    std::vector<Message>::iterator iter;
    for (iter = messages.begin(); iter != messages.end(); ++iter)
    {
        output(EVENTS_OUTLET, *iter);
    }
}

} // end of namespace
} // end of namespace


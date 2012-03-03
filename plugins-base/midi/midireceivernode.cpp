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
#include <iostream>

namespace tempi {
namespace plugins_base {

MidiReceiverNode::MidiReceiverNode() :
    Node()
{
    setShortDocumentation("Receives MIDI messages from a single device.");
    addOutlet("0", "MIDI messages (unsigned characters) flow through this outlet.");
    Message port = Message("i", 0);
    addAttribute(Attribute::ptr(new Attribute("port", port, "STK MIDI device index.")));
    midi_input_.reset(new midi::MidiInput);
}

void MidiReceiverNode::onAttributeChanged(const char *name, const Message &value)
{
    //std::cout << "MidiReceiverNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    static std::string key("port");
    if (key == name)
    {
        //std::cout << "MidiInput::" << __FUNCTION__ << " open port " << value.getInt(0) << std::endl;
        midi_input_->open((unsigned int) value.getInt(0));
    }
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
        output("0", *iter);
    }
}

} // end of namespace
} // end of namespace


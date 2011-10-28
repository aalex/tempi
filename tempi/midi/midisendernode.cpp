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

#include "tempi/midi/midisendernode.h"
#include <iostream>

namespace tempi
{

namespace midi
{

MidiSenderNode::MidiSenderNode() :
    Node()
{
    addOutlet();
    Message port = Message("i", 0);
    addProperty("port", port);
    midi_output_.reset(new MidiOutput);
}

void MidiSenderNode::onPropertyChanged(const char *name, const Message &value)
{
    //std::cout << "MidiSenderNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    static std::string key("port");
    if (key == name)
    {
        //std::cout << "MidiSenderNode::" << __FUNCTION__ << " open port " << value.getInt(0) << std::endl;
        midi_output_->open((unsigned int) value.getInt(0));
    }
}

// TODO: output the list of devices upon query

void MidiSenderNode::processMessage(unsigned int inlet, const Message &message)
{
    if (midi_output_->isOpen())
    {
        bool ok = midi_output_->sendMessage(message);
        if (! ok)
            std::cout << "MidiSenderNode::" << __FUNCTION__ << ": Error sending " << message << std::endl;
    }
    else
        std::cout << "MidiSenderNode::" << __FUNCTION__ << ": MIDI output is not open. Cannot send " << message << std::endl;
}

} // end of namespace

} // end of namespace


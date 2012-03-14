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

#include "plugins-base/midi/midibuildernodes.h"

namespace tempi{
namespace plugins_base {

static int clip(int value, int from, int to);

NoteBuilderNode::NoteBuilderNode() :
    AbstractMidiEventBuilderNode()
{
    setShortDocumentation("Build note out of a list of integers");
    note_ = 0;
    velocity_ = 0;
}

int clip(int value, int from, int to)
{
    if (value < from)
        return from;
    else if (value > to)
        return to;
    else
        return value;
}
bool NoteBuilderNode::buildMidiEvent(const std::vector<int>& ints, Message &result)
{
    
    switch(ints.size())
    {
        case 0:
            break;
        case 1:
            note_ = ints[0];
            break;
        case 2:
            note_ = ints[0];
            velocity_ = ints[1];
        case 3:
            note_ = ints[0];
            velocity_ = ints[1];
            Message channel;
            channel.appendInt(ints[2]);
            this->setAttributeValue(CHANNEL_ATTR, channel);
    }

    result.appendUnsignedChar((unsigned char) this->getAttributeValue(CHANNEL_ATTR).getInt(0) - 1 + 0x90);
    result.appendUnsignedChar((unsigned char) clip(note_, 0, 127));
    result.appendUnsignedChar((unsigned char) clip(velocity_, 0, 127));
    return true;
}
} // end of namespace
} // end of namespace


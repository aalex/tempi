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
    if (ints.size() == 2)
    {
        result.appendUnsignedChar((unsigned char) this->getAttributeValue(CHANNEL_ATTR).getInt(0) - 1 + 0x90);
        result.appendUnsignedChar((unsigned char) clip(ints[0], 0, 127));
        result.appendUnsignedChar((unsigned char) clip(ints[1], 0, 127));
        return true;
    }
}
} // end of namespace
} // end of namespace


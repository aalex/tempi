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

#include "plugins-base/midi/midibuildernodes.h"
#include "tempi/log.h"

namespace tempi{
namespace plugins_base {


const char * const ControlBuilderNode::CTL_ATTR = "controller"; // set controller number attribute
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
    
    result.appendInt(this->getAttributeValue(CHANNEL_ATTR).getInt(0) - 1 + 0x90);
    result.appendInt(clip(note_, 0, 127));
    result.appendInt(clip(velocity_, 0, 127));
    return true;
}

ControlBuilderNode::ControlBuilderNode() :
    AbstractMidiEventBuilderNode()
{
    addAttribute(Attribute::ptr(new Attribute(CTL_ATTR, Message("i", 0), "Controller number (from 0 to 127)")));
    setShortDocumentation("Build controller message out of a list of integers");
    value_ = 0;
}

bool ControlBuilderNode::buildMidiEvent(const std::vector<int>& ints, Message &result)
{
    Message controller;
    
    switch(ints.size())
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            value_ = ints[0];
            break;
        }
        case 2:
        {
            value_ = ints[0];
            controller.appendInt(clip(ints[1], 0 , 127));
            this->setAttributeValue(CTL_ATTR, controller);
            break;
        }
        case 3:
        {
            value_ = ints[0];
            controller.appendInt(clip(ints[1], 0 , 127));
            this->setAttributeValue(CTL_ATTR, controller);
            Message channel;
            channel.appendInt(ints[2]);
            this->setAttributeValue(CHANNEL_ATTR, channel);
            break;
        }
        default:
        {
            return false; // wrong number of args
            break;
        }
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "ControlBuilderNode." << __FUNCTION__ <<
            ": channel=" << this->getAttributeValue(CHANNEL_ATTR) <<
            " controller=" << this->getAttributeValue(CTL_ATTR) <<
            " value=" << value_;
        Logger::log(DEBUG, os);
    }

    result.appendInt(this->getAttributeValue(CHANNEL_ATTR).getInt(0) - 1 + 0xB0);
    result.appendInt(this->getAttributeValue(CTL_ATTR).getInt(0));
    result.appendInt(clip(value_, 0, 127));
    return true;
}

ProgramChangeBuilderNode::ProgramChangeBuilderNode() :
    AbstractMidiEventBuilderNode()
{
    setShortDocumentation("Build a program change message out of an integer");
    program_ = 0;
}

bool ProgramChangeBuilderNode::buildMidiEvent(const std::vector<int>& ints, Message &result)
{
    
    switch(ints.size())
    {
        case 0:
            break;
        case 1:
            program_ = ints[0];
            break;
        case 2:
            program_ = ints[0];
            Message channel;
            channel.appendInt(ints[2]);
            this->setAttributeValue(CHANNEL_ATTR, channel);
    }

    result.appendInt((unsigned char) this->getAttributeValue(CHANNEL_ATTR).getInt(0) - 1 + 0xC0);
    result.appendInt((unsigned char) clip(program_, 0, 127));
    return true;
}
} // end of namespace
} // end of namespace


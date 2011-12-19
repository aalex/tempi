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

#include <iostream>
#include "tempi/base/prependnode.h"

namespace tempi { namespace base {

PrependNode::PrependNode() :
    Node(),
    prefix_()
{
    addOutlet();
}

void PrependNode::processMessage(unsigned int inlet, const Message &message)
{
    Message ret = message;
    for (unsigned int i = prefix_.getSize(); i >= 0; --i)
    {
        ArgumentType type;
        prefix_.getArgumentType(i, type);
        switch (type)
        {
            case BOOLEAN:
                ret.prependBoolean(prefix_.getBoolean(i));
                break;
            case CHAR:
                ret.prependBoolean(prefix_.getBoolean(i));
                break;
            case DOUBLE:
                ret.prependDouble(prefix_.getDouble(i));
                break;
            case FLOAT:
                ret.prependFloat(prefix_.getFloat(i));
                break;
            case INT:
                ret.prependInt(prefix_.getInt(i));
                break;
            case LONG:
                ret.prependLong(prefix_.getLong(i));
                break;
            case STRING:
                ret.prependString(prefix_.getString(i).c_str());
                break;
            default:
                std::cerr << "base::PrependNode: Unknow type of atom: " << type << std::endl;
                break;
        }
    }
    output(0, ret);
}

void PrependNode::onSetArguments(const Message &message)
{
    prefix_ = message;
}

} // end of namespace
} // end of namespace


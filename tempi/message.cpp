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

#include "tempi/message.h"
#include <iostream>

namespace tempi
{

Message::Message(std::vector<boost::any> arguments)
{
    arguments_ = arguments;
}

Message::Message()
{
    // pass
}

bool Message::append(boost::any value)
{
    arguments_.push_back(value);
}

std::vector<boost::any> &Message::getArguments()
{
    return arguments_;
}

// std::vector<std::type_info> Message::getTypes()
// {
//     std::vector<std::type_info> ret;
//     typename std::vector<boost::any>::iterator iter;
//     for (iter = arguments_.begin(); iter < arguments_.end(); ++iter)
//     {
//         ret.push_back((*iter).type());
//     }
//     return ret;
// }

unsigned int Message::getSize() const
{
    return arguments_.size();
}

bool Message::setArgument(unsigned int index, boost::any &value)
{
    if (index >= getSize())
    {
        std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
        return false;
    }
//    else if (value.type() != arguments_[index].type())
//    {
//        std::cerr << "Message::" << __FUNCTION__ << ": Wrong type for index " << index << " " << value.type().name() << std::endl;
//        return false;
//    }
    else
    {
        arguments_[index] = value;
        return true;
    }
}

boost::any *Message::getArgument(unsigned int index)
{
    if (index >= getSize())
    {
        std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
        return false;
    }
    else
    {
        return &arguments_[index];
    }
}


const std::type_info *Message::getType(unsigned int index) const
{
    if (index >= getSize())
    {
        std::cerr << "Message::" << __FUNCTION__ << ": Index too big: " << index << std::endl;
        return 0;
    }
    else
    {
        return &(arguments_[index].type());
    }
}

} // end of namespace


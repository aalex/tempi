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

#include "tempi/base/appsinknode.h"

namespace tempi { namespace base {

AppSinkNode::AppSinkNode() :
    Node()
{
    addOutlet();
}

void AppSinkNode::processMessage(unsigned int inlet, const Message &message)
{
    queue_.push(message);
}

bool AppSinkNode::tryPop(Message &message)
{
    return queue_.try_pop(message);
}

bool AppSinkNode::isEmpty() const
{
    return queue_.empty();
}

void AppSinkNode::clear()
{
    queue_.clear();
}

void AppSinkNode::push(const Message &message)
{
    queue_.push(message);
}

} // end of namespace
} // end of namespace


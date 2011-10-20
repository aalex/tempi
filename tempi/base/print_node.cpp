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
#include "tempi/base/print_node.h"

namespace tempi
{

PrintNode::PrintNode() :
    Node()
{
    addOutlet();
    // add the "prefix" string property
    Message value = Message("s", "print: ");
    addProperty("prefix", value);
}

void PrintNode::processMessage(unsigned int inlet, const Message &message)
{
    std::cout << getProperty("prefix").getString(0) << message << std::endl;
}

// void PrintNode::onPropertyChanged(const char *name, const Message &value)
// {
//     const static name_key("");
// }

} // end of namespace


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

#include <iostream>
#include "tempi/base/loadmessnode.h"
#include "tempi/log.h"

namespace tempi {
namespace base {

LoadMessNode::LoadMessNode() :
    Node()
{
    this->setShortDocumentation("Loads a message at the init of the Graph.");
    this->addOutlet("0", "The default outlet.");
    this->addAttribute(Attribute::ptr(new Attribute("value", Message(), "Value to output at startup. Set it to whatever you wish.", false)));
}

void LoadMessNode::processMessage(const char *inlet, const Message &message)
{
    // pass
}

void LoadMessNode::onLoadBang()
{
    this->output("0", this->getAttributeValue("value"));
    std::ostringstream os;
    os << "LoadMessNode::" << __FUNCTION__ << ": Node::output(" << this->getAttributeValue("value") << ")";
    Logger::log(DEBUG, os.str().c_str());
}

} // end of namespace
} // end of namespace


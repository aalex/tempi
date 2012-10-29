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

#include "plugins-base/flow/counternode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

const char * const CounterNode::PROP_INCREMENT = "increment";
const char * const CounterNode::PROP_INITIAL_COUNT = "initial_count";

CounterNode::CounterNode() :
    Node()
{
    setShortDocumentation("Counts from its start value, incrementing or decrementing the count when it gets a bang.");
    addOutlet("0", "Count.");
    addInlet("0", "Bangs increase and output the count. Integers sets and outputs the count.");
    Message increment = Message("i", 1);
    addAttribute(Attribute::ptr(new Attribute(PROP_INCREMENT, increment)));
    Message ini_count_attr = Message("i", 0);
    addAttribute(Attribute::ptr(new Attribute(PROP_INITIAL_COUNT, ini_count_attr, "Initial count value.")));

    this->count_ = 0;

    {
        std::ostringstream os;
        os << "CounterNode::" << __FUNCTION__ << "(): count_ = " << this->count_;
        Logger::log(INFO, os);
    }
}

void CounterNode::processMessage(const char *inlet, const Message &message)
{
    // TODO: check if inlet == "0"
    // bang outputs and increments
    if (message.getTypes() == "")
    {
        int increment = getAttributeValue(PROP_INCREMENT).getInt(0);
        {
            std::ostringstream os;
            os << "CounterNode::" << __FUNCTION__ << "(" << inlet << ", " << message << "): count_ = " << this->count_;
            Logger::log(INFO, os);
        }
        output("0", Message("i", count_));
        count_ += increment;
    }
    // int replaces the value and outputs
    else if (message.getTypes() == "i")
    {
        count_ = message.getInt(0);
        setAttributeValue(PROP_INITIAL_COUNT, message);
        output("0", getAttributeValue(PROP_INITIAL_COUNT));
    }
    else
    {
        std::cerr << "CounterNode::" << __FUNCTION__ << ": No method for " << message << std::endl;
    }
}

bool CounterNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(PROP_INITIAL_COUNT, name))
    {
        count_ = value.getInt(0);
        {
            std::ostringstream os;
            os << "CounterNode::" << __FUNCTION__ << "(" << name << ", " << value << "): count_ = " << this->count_;
            Logger::log(INFO, os);
        }
    }
    return true;
}

} // end of namespace
} // end of namespace


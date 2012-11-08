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

#include <iostream>
#include "plugins-base/flow/splitnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const SplitNode::INPUT_INLET = "input";
const char * const SplitNode::FIRST_N_OUTLET = "first_n";
const char * const SplitNode::LEFTOVER_OUTLET = "leftover";
const char * const SplitNode::TOOSHORT_OUTLET = "too_short";
const char * const SplitNode::N_ATTR = "n";

SplitNode::SplitNode() :
    Node()
{
    this->addAttribute(Attribute::ptr(new Attribute(N_ATTR, Message("i", 0), "How many values to output to the first_n outlet.")));
    this->addInlet(INPUT_INLET, "Incoming messages go here.");
    this->setShortDocumentation("Splits messages, outputs first_n values and leftover.");
    this->addOutlet(FIRST_N_OUTLET, "First n values.");
    this->addOutlet(LEFTOVER_OUTLET, "Leftover values.");
    this->addOutlet(TOOSHORT_OUTLET, "Messages that are too short are output here instean.");
}

void SplitNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, INPUT_INLET))
        return;
    unsigned int split_point = (unsigned int) this->getAttributeValue(N_ATTR).getInt(0);
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[split] " << __FUNCTION__ << ": split point " << split_point;
        Logger::log(DEBUG, os);
    }
    unsigned int message_size = message.getSize();

    if (message_size < split_point)
    {
        this->output(TOOSHORT_OUTLET, message);
        return;
    }
    Message first_n = message.cloneRange(0, split_point - 1);
    if (message_size > split_point)
    {
        Message leftover = message.cloneRange(split_point, message_size - 1);
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "[split] " << __FUNCTION__ << ": output leftover " << leftover;
            Logger::log(INFO, os);
        }
        this->output(LEFTOVER_OUTLET, leftover);
    }
    else
    {
        Message leftover;
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "[split] " << __FUNCTION__ << ": output leftover " << leftover;
            Logger::log(INFO, os);
        }
        this->output(LEFTOVER_OUTLET, leftover);
    }
    this->output(FIRST_N_OUTLET, first_n);
}

bool SplitNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    return true; // true means it's OK to change that attribute
}

} // end of namespace
} // end of namespace


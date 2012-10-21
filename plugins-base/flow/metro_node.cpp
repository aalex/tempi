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

#include "plugins-base/flow/metro_node.h"
#include "tempi/timeposition.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

static const char * const RUNNING_ATTR = "running";
static const char * const INTERVAL_ATTR = "interval";

MetroNode::MetroNode() :
    Node(),
    initial_tick_done_(false)
{
    this->setShortDocumentation("Sends an empty message every n milliseconds.");
    Message running = Message("b", false);
    this->addAttribute(Attribute::ptr(new Attribute(RUNNING_ATTR, running, "Whether it's ticking or not.")));

    Message interval = Message("i", 1000);
    this->addAttribute(Attribute::ptr(new Attribute(INTERVAL_ATTR, interval, "Interval in milliseconds."))); // ms

    this->addOutlet("0");
}

bool MetroNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    static const std::string RUNNING(RUNNING_ATTR);

    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "MetroNode::" << __FUNCTION__ << ": " << name << " = " << value;
        Logger::log(DEBUG, os);
    }
    if (RUNNING == name)
    {
        if (value.getBoolean(0))
        {
            this->startMetro();
            // XXX: do not output tick here, since nodes are not yet connection. We do this in doTick()
        }
    }
    return true;
}

void MetroNode::startMetro()
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "MetroNode::" << __FUNCTION__;
        Logger::log(DEBUG, os);
    }
    this->initial_tick_done_ = false;
    timer_.reset();
    if (this->isInitiated())
        this->outputTick();
}

void MetroNode::outputTick()
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "MetroNode::" << __FUNCTION__;
        Logger::log(DEBUG, os);
    }
    this->initial_tick_done_ = true;
    Message message = Message("s", "hello"); // bang
    this->output("0", message);
}

void MetroNode::doTick()
{
    using timeposition::from_ms;
    using timeposition::to_ms;

    if (getAttributeValue(RUNNING_ATTR).getBoolean(0))
    {
        if (! this->initial_tick_done_)
        {
            if (this->isInitiated())
                this->outputTick();
            return;
        }
        TimePosition interval = from_ms((unsigned long long) getAttributeValue("interval").getInt(0));
        TimePosition elapsed = timer_.elapsed();
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "MetroNode::" << __FUNCTION__ << " interval:" << to_ms(interval) <<
                " elapsed:" << elapsed;
            Logger::log(DEBUG, os);
        }
        if (elapsed >= interval)
        {
            this->outputTick();
            timer_.reset();
        }
    }
}

} // end of namespace
} // end of namespace


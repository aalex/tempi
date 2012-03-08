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

#include "plugins-base/flow/linenode.h"
#include <algorithm>
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const LineNode::OUTLET_RAMP = "ramp";
const char * const LineNode::INLET_TARGETS = "targets";
const char * const LineNode::INLET_CLEAR = "clear";
const char * const LineNode::ATTR_RATE = "rate";


static TimePosition floatToTimePosition(float f);

LineNode::LineNode() :
    Node()
{
    setShortDocumentation("Generates ramps.");
    Message rate;
    rate.appendLong(20);
    addAttribute(Attribute::ptr(new Attribute(ATTR_RATE, rate, "Interval between each output in milliseconds.")));
    //addAttribute(Attribute::ptr(new Attribute("target", Message("f", 0.0f), "Target.")));
    //addAttribute(Attribute::ptr(new Attribute("time", Message("f", 0.0f), "Time before reaching target.")));
    addInlet(INLET_TARGETS, "Triggers the ramp generation. Expects a list of an even number of floats.");
    addInlet(INLET_CLEAR, "Send anything to this inlet to clear the ramps.");
    addOutlet(OUTLET_RAMP, "Generated ramp.");
}

void LineNode::computeTargets(const Message &message)
{
    {
        Logger::Output os;
        os << "[line]: " << __FUNCTION__ << ": " << message;
        Logger::log(INFO, os);
    }
    unsigned int size = message.getSize();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (! message.indexMatchesType(0, 'f'))
        {
            Logger::Output os;
            os << "[line]: Invalid atom type for targets: " << message;
            Logger::log(ERROR, os);
            return;
        }
    }
    if (size == 0)
    {
        Logger::Output os;
        os << "[line]: Invalid message length for targets: " << message;
        Logger::log(ERROR, os);
        return;
    }

    targets_.clear();
    rate_timer_.reset(); // very important!!
    timer_.reset();

    if (size == 1)
    {
        float value = message.getFloat(0);
        this->output(OUTLET_RAMP, Message("f", value)); // Jump to target
        {
            Logger::Output os;
            os << "[line]: Jump to target: " << value;
            Logger::log(INFO, os);
        }
        origin_ = value;
        return;
    }

    // else
    // size >= 2
    {
        for (unsigned int i; i < size; i = i + 2)
        {
            float target = message.getFloat(i);
            bool has_duration = (size > (i + 1));
            TimePosition duration = 0L;
            if (has_duration)
            {
                duration = floatToTimePosition(message.getFloat(i + 1));
            }
            Logger::Output os;
            os << "[line]: add target: " << target << " " << duration;
            Logger::log(INFO, os);

            targets_.push_back(Target(target, duration));
        }
    }
}

bool LineNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(ATTR_RATE, name))
    {
        // not needed here: rate_timer_.reset();
    }
    return true;
}

TimePosition floatToTimePosition(float f)
{
    return timeposition::from_ms((unsigned long long) f);
}

void LineNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, INLET_TARGETS))
    {
        computeTargets(message); // message.cloneRange(1, message.getSize() + 1));
    }
    else if (utils::stringsMatch(inlet, INLET_CLEAR))
    {
        targets_.clear();
    }
}

void LineNode::doTick()
{
    using timeposition::from_ms;
    using timeposition::to_ms;

    if (targets_.size() == 0)
    {
        Logger::Output os;
        os << "[line]: No more target.";
        Logger::log(DEBUG, os);
        return;
    }
    TimePosition rate = from_ms(
        (unsigned long long) getAttributeValue(ATTR_RATE).getLong(0));

    if (rate_timer_.elapsed() < rate)
    {
        Logger::Output os;
        os << "[line]: Not yet time to output a new value.";
        Logger::log(INFO, os);
        return;
    }
    else
    {
        Logger::Output os;
        os << "[line]: time to output a new value since last rate: " << rate_timer_.elapsed();
        Logger::log(INFO, os);
    }
    rate_timer_.reset(); // very important!!
    TimePosition elapsed = timer_.elapsed();
    TimePosition duration = targets_[0].get<1>();
    float target = targets_[0].get<0>();
    if (elapsed >= duration || duration == 0L)
    {
        {
            Logger::Output os;
            os << "[line]: Reached a new target! time=" <<
                duration << " target=" << target;
            Logger::log(INFO, os);
        }
        this->output(OUTLET_RAMP, Message("f", target));
        targets_.erase(targets_.begin());
        rate_timer_.reset(); // IMPORTANT
        timer_.reset();
        origin_ = target;
    }
    else
    {
        float current = utils::map_float(
            (float) ((float) elapsed / (float) duration), // duration != 0L
            0.0f, (float) duration,
            origin_, target);
        {
            Logger::Output os;
            os << "[line]: map " << ((float) elapsed / (float) duration) << 
                "  from [0, " << (float) duration << "] to [" <<
                origin_ << ", " << target << "] = " << current;
            Logger::log(INFO, os);
        }
        this->output(OUTLET_RAMP, Message("f", current));
    }
}

} // end of namespace
} // end of namespace


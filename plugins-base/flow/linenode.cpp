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
static float timePositionToFloat(TimePosition f);

LineNode::LineNode() :
    Node()
{
    setShortDocumentation("Generates ramps.");
    Message rate;
    rate.appendInt(20);
    addAttribute(Attribute::ptr(new Attribute(ATTR_RATE, rate, "Interval between each output in milliseconds.")));
    //addAttribute(Attribute::ptr(new Attribute("target", Message("f", 0.0f), "Target.")));
    //addAttribute(Attribute::ptr(new Attribute("time", Message("f", 0.0f), "Time before reaching target.")));
    addInlet(INLET_TARGETS, "Triggers the ramp generation. Expects a list of an even number of floats.");
    addInlet(INLET_CLEAR, "Send anything to this inlet to clear the ramps.");
    addOutlet(OUTLET_RAMP, "Generated ramp.");
}

void LineNode::computeTargets(const Message &message)
{
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

    if (size == 1 && message.indexMatchesType(0, FLOAT))
    {
        float value = message.getFloat(0);
        line_.jumpTo(value);
        this->output(OUTLET_RAMP, Message("f", value)); // Jump to target
        if (Logger::isEnabledFor(DEBUG))
        {
            Logger::Output os;
            os << "[line]: Jump to target: " << value;
            Logger::log(DEBUG, os);
        }
        return;
    }
    else
    {
        // IMPORTANT!!!
        this->doTick();
    }

    // else
    // size >= 2
    {
        for (unsigned int i; i < size; i = i + 2)
        {
            if (! message.indexMatchesType(i, FLOAT))
            {
                Logger::log(ERROR, "line: wrong type for arg.");
                return;
            }
            float target = message.getFloat(i);
            bool has_duration = (size > (i + 1));
            TimePosition duration = 0L;
            if (has_duration)
            {
                if (! message.indexMatchesType(i + 1, FLOAT))
                {
                    Logger::log(ERROR, "line: wrong type for arg.");
                    return;
                }
                float ms = message.getFloat(i + 1);
                duration = floatToTimePosition(ms);

                if (Logger::isEnabledFor(INFO))
                {
                    Logger::Output os;
                    os << "[line]: add target: " << target << " " << ms;
                    Logger::log(INFO, os);
                }
            }
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

float timePositionToFloat(TimePosition pos)
{
    return (float) timeposition::to_ms(pos);
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

bool LineNode::tryPopTarget(Target &result)
{
    if (targets_.size() == 0)
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            Logger::Output os;
            os << "[line]: No more target.";
            Logger::log(DEBUG, os);
        }
        return false;
    }
    result = targets_[0];
    targets_.erase(targets_.begin());
    {
        if (Logger::isEnabledFor(INFO))
        {
            Logger::Output os;
            os << "[line]: target " << result.get<0>() << " " << result.get<1>();
            Logger::log(INFO, os);
        }
    }
    true;
}

bool LineNode::isTimeToOutput()
{
    TimePosition rate = timeposition::from_ms(
        (unsigned long long) getAttributeValue(ATTR_RATE).getInt(0));

    if (rate_timer_.elapsed() > rate)
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            Logger::Output os;
            os << "[line]: Not yet time to output a new value.";
            Logger::log(DEBUG, os);
        }
        return false;
    }
    else
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            Logger::Output os;
            os << "[line]: time to output a new value since last rate: " << rate_timer_.elapsed();
            Logger::log(DEBUG, os);
        }
    }
    rate_timer_.reset(); // very important!!
    return true;
}

void LineNode::doTick()
{
    if (! isTimeToOutput())
        return;

    bool output_something = false;
    if (line_.isAlreadyArrived())
    {
        Target target;
        bool has_new_target = tryPopTarget(target);
        if (has_new_target)
        {
            output_something = true;
            line_.start(target.get<0>(), target.get<1>());
        }
        else
            output_something = false;
    }
    else
    {
        output_something = true;
    }

    if (output_something)
    {
        float current = line_.calculateCurrent();
        this->output(OUTLET_RAMP, Message("f", current));
    }
}

Line::Line() :
    origin_(0.0f),
    target_(0.0f),
    position_(0.0f),
    duration_(0L)
{
    timer_.reset();
}

void Line::jumpTo(float target)
{
    origin_ = target;
    target_ = target;
    position_ = target;
    duration_ = 0L;
    timer_.reset();
}

float Line::getOrigin() const
{
    return origin_;
}

float Line::getTarget() const
{
    return target_;
}

float Line::getDuration() const
{
    return timePositionToFloat(duration_);
}

void Line::start(float target, float duration_ms)
{
    if (duration_ms == 0.0f)
    {
        jumpTo(target);
        return;
    }
    origin_ = calculateCurrent(); // position_
    target_ = target;
    duration_ = floatToTimePosition(duration_ms);
    timer_.reset();
    if (Logger::isEnabledFor(NOTICE))
    {
        Logger::Output os;
        os << "Line.start(" << target_ << ", " << duration_ms <<
            " ... duration_=" << duration_ <<
            " origin_=" << origin_;
        Logger::log(NOTICE, os);
    }
}

float Line::calculateCurrent()
{
    if (this->isAlreadyArrived())
        return target_;
    TimePosition elapsed = timer_.elapsed();

    if (duration_ == 0L)
        return target_; // avoid division by zero
    float progress = (float) elapsed / (float) duration_; // both in ns, thus progress has no unit
    float current = utils::map_float(
        progress,
        0.0f, 1.f, // map from [start, end], according to progress
        origin_, target_);       // to [origin, target] (whatever value)
    if (Logger::isEnabledFor(INFO))
    {
        Logger::Output os;
        os << "Line: map " << progress <<
            "  from [0, " << (float) duration_ << "] to [" <<
            origin_ << ", " << target_ << "] = " << current;
        Logger::log(INFO, os);
    }
    return current;
}

bool Line::isAlreadyArrived()
{
    if (timer_.elapsed() > duration_)
        return true;
    if (duration_ == 0L)
        return true;
    return false;
}

} // end of namespace
} // end of namespace


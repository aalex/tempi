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

#include "plugins-base/flow/speedlimnode.h"
#include "tempi/timeposition.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const SpeedLimNode::INTERVAL_ATTR = "interval";
const char * const SpeedLimNode::MSG_INLET = "in";
const char * const SpeedLimNode::MSG_OUTLET = "out";

SpeedLimNode::SpeedLimNode() :
    Node()
{
    this->setShortDocumentation("Limits the data rate of messages flowing through it.");
    this->addAttribute(Attribute::ptr(new Attribute(INTERVAL_ATTR, Message("i", 20), "The minimal time duration (in milliseconds) between each message.")));

    this->addInlet(MSG_INLET);
    this->addOutlet(MSG_OUTLET);

    this->timer_.reset();
    this->message_to_output_ = false;
}

void SpeedLimNode::outputLastMessageIfNeeded()
{
    if (this->message_to_output_)
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "SpeedLimNode::" << __FUNCTION__;
            os << ": something to output.";
            Logger::log(DEBUG, os);
        }
        this->output(MSG_OUTLET, this->last_message_);
        this->message_to_output_ = false;
        this->timer_.reset();
    }
    else
    {
        return;
    }
}

void SpeedLimNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, MSG_INLET))
    {
        // outputs right aways and resets the timer
        if (this->isTimeToOutput())
        {
            this->output(MSG_OUTLET, message);
            this->message_to_output_ = false;
            timer_.reset();
        }
        else
        {
            // store to output it later
            this->last_message_ = message;
            this->message_to_output_ = true;
        }
    }
}

bool SpeedLimNode::isTimeToOutput()
{
    using timeposition::from_ms;
    using timeposition::to_ms;

    TimePosition interval = from_ms((unsigned long long) getAttributeValue(INTERVAL_ATTR).getInt(0));
    TimePosition elapsed = timer_.elapsed();
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "SpeedLimNode::" << __FUNCTION__ << " interval:" << to_ms(interval) <<
            " elapsed:" << elapsed;
        Logger::log(DEBUG, os);
    }
    return (elapsed >= interval);
}

void SpeedLimNode::doTick()
{
    if (this->isTimeToOutput())
    {
        this->outputLastMessageIfNeeded();
    }
}

} // end of namespace
} // end of namespace


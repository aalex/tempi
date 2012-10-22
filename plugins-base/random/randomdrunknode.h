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

/**
 * @file
 * The RandomDrunkNode class.
 */
#ifndef __TEMPI_RANDOM_DRUNKNODE_H__
#define __TEMPI_RANDOM_DRUNKNODE_H__

#include "tempi/node.h"
#include "tempi/utils.h"
#include "plugins-base/random/drunkint.h"

namespace tempi {
namespace plugins_base {

/**
 * The RandomDrunkNode outputs random integers following a random walk with a certain range.
 */
class RandomDrunkNode : public Node
{
    public:
        RandomDrunkNode() :
            Node()
        {
            this->setShortDocumentation("Outputs random integers following a random walk with a certain range");
            this->addInlet("0", "Triggers output when it receives any message.");
            this->addOutlet("0", "From where random integer numbers flow.");
            this->addAttribute(Attribute::ptr(new Attribute("current", Message("i", 0), "Current value")));
            this->addAttribute(Attribute::ptr(new Attribute("range", Message("ii", 0, 1), "Bounds of the range of output numbers.")));
            this->addAttribute(Attribute::ptr(new Attribute("step", Message("ii", 0, 1), "Minimum size of the steps")));
        }
    protected:
        virtual bool onNodeAttributeChanged(const char *name, const Message &value)
        {
            {
                std::ostringstream os;
                os << "RandomDrunkNode." << __FUNCTION__ << ": " << name << "=" << value;
                Logger::log(INFO, os.str().c_str());
            }
            if (utils::stringsMatch(name, "current"))
            {
                if (! drunk_.setCurrent(value.getInt(0)))
                {
                    Logger::log(WARNING, "[random.drunk]: Failed to set current");
                    return false;
                }
            }
            else if (utils::stringsMatch(name, "range"))
            {
                if (! drunk_.setRange(value.getInt(0), value.getInt(1)))
                {
                    Logger::log(WARNING, "[random.drunk]: Failed to set range");
                    return false;
                }
            }
            else if (utils::stringsMatch(name, "step"))
            {
                if (! drunk_.setStepRange(value.getInt(0), value.getInt(1)))
                {
                    Logger::log(WARNING, "[random.drunk]: Failed to set step");
                }
            }
            return true;
        }
    private:
        DrunkInt drunk_;
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (utils::stringsMatch(inlet, "0"))
            {
                int current = drunk_.step();
                if (false)
                {
                    std::ostringstream os;
                    os << "RandomDrunkNode::" << __FUNCTION__ << ": current=" << drunk_.getCurrent() << " range=[" << drunk_.getRangeFrom() << "," << drunk_.getRangeTo() << "] step=[" << drunk_.getStepRangeMin() << "," << drunk_.getStepRangeMax() << "]";
                    Logger::log(DEBUG, os.str().c_str());
                }
                getAttribute("current")->setValue(Message("i", current));
                output("0", getAttribute("current")->getValue());
            }
        }
};

} // end of namespace
} // end of namespace

#endif // ifndef


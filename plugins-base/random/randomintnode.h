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

/**
 * @file
 * The RandomIntNode class.
 */
#ifndef __TEMPI_RANDOM_INTNODE_H__
#define __TEMPI_RANDOM_INTNODE_H__

#include "tempi/node.h"
#include "tempi/utils.h"
#ifdef HAVE_GLIB
#include <glib.h>
#endif

namespace tempi {
namespace plugins_base {

/**
 * The RandomIntNode outputs random integers within a certain range.
 */
class RandomIntNode : public Node
{
    public:
        RandomIntNode() :
            Node()
        {
            this->setShortDocumentation("Outputs random integers within a given range");
            this->addInlet("0", "Triggers output when it receives any message.");
            this->addOutlet("0", "From where random integer numbers flow.");
            this->addAttribute(Attribute::ptr(new Attribute("range", Message("ii", 0, 1), "Bounds of the range of output numbers.")));
        }
    protected:
        virtual bool onNodeAttributeChanged(const char *name, const Message &value)
        {
            {
                std::ostringstream os;
                os << "RandomIntNode." << __FUNCTION__ << ": " << name << "=" << value;
                Logger::log(DEBUG, os.str().c_str());
            }
            if (utils::stringsMatch(name, "range"))
            {
                if (! drunk_.setRange(value.getInt(0), value.getInt(1)))
                {
                    //setAttributeValue("range", Message("ii", drunk_.getRangeFrom(), drunk_.getRangeTo()));
                    Logger::log(WARNING, "[random.drunk]: Failed to set range");
                }
            }
        }
    private:
        DrunkInt drunk_;
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (utils::stringsMatch(inlet, "0"))
            {
#ifdef HAVE_GLIB
                // TODO: make drunk steps configurable
                gint32 result = g_random_int_range(
                    this->getAttributeValue("range").getInt(0),
                    this->getAttributeValue("range").getInt(1) + 1);
#else
                std::cerr << "RandomIntNode: disabled since compiled without glib support." << std::endl;
                gint32 result = 0;
#endif
                output("0", Message("i", result));
            }
        }
};

} // end of namespace
} // end of namespace

#endif // ifndef


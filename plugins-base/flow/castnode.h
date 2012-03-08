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
 * The CastNode class.
 */
#ifndef __TEMPI_BASE_CASTNODE_H__
#define __TEMPI_BASE_CASTNODE_H__

#include "tempi/node.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

/**
 * The CastNode tries to cast incoming messages to a given type
 */
class CastNode : public Node
{
    public:
        CastNode() :
            Node(),
            desired_type_("")
        {
            this->setShortDocumentation("Tries to cast incoming messages to a given type");
            this->addOutlet("0", "The outlet for messages casted to the desired type, if successful.");
            this->addOutlet("success", "Whether or not last cast was succesful.");
            this->addInlet("0", "The inlet for incoming messages to cast.");
            this->addAttribute(Attribute::ptr(new Attribute("type", Message("s", ""), "The desired type tag.")));
        }
    private:
        std::string desired_type_;
        virtual bool onNodeAttributeChanged(const char *name, const Message &value)
        {
            if (utils::stringsMatch("type", name))
            {
                if (utils::isValidType(value.getString(0).c_str()))
                    desired_type_ = value.getString(0);
                else
                {
                    // back to previous
                    setAttributeValue("type", Message("s", desired_type_.c_str()));
                }
            }
        }
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (! utils::stringsMatch(inlet, "0"))
                return;
            try
            {
                Message result = utils::castMessage(message, desired_type_.c_str());
                this->output("0", result);
            }
            catch (const BadAtomTypeException &e)
            {
                Logger::log(WARNING, e.what());
            }
            catch (const BadIndexException &e)
            {
                Logger::log(WARNING, e.what());
            }
        }
};

} // end of namespace
} // end of namespace

#endif // ifndef


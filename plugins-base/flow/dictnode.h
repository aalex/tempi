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
 * The RandomDrunkNode class.
 */
#ifndef __TEMPI_BASE_DICTNODE_H__
#define __TEMPI_BASE_DICTNODE_H__

#include "tempi/node.h"
#include "tempi/utils.h"
#include "tempi/dict.h"

namespace tempi {
namespace plugins_base {
namespace flow {

/**
 * Stores key-value pairs.
 */
class DictNode : public Node
{
    public:
        DictNode() :
            Node()
        {
            this->setShortDocumentation("Stores key-value pairs");
            this->addInlet("get", "Triggers output when it receives any message starting with a string.");
            this->addInlet("set", "Sets values. Messages must start by a string");
            this->addInlet("remove", "Erases values. Messages must start by a string");
            this->addInlet("list", "Bang this inlet to list keys thought the \"list\" outlet.");
            this->addInlet("reset", "Bang this inlet to remove all keys of the dict.");
            this->addOutlet("0", "From where values are output.");
            this->addOutlet("list", "From where list of keys are sent when the \"list\" inlet in banged.");
        }
    private:
        dict::Dict dict_;
        virtual void processMessage(const char *inlet, const Message &message)
        {
            //Logger::log(ERROR, "[base.dict]: get method expects a string as first atom.");
            if (utils::stringsMatch(inlet, "get"))
            {
                if (message.indexMatchesType(0, 's'))
                {
                    Message value;
                    dict_.get(message.getString(0).c_str(), value);
                    output("0", value);
                }
                else
                {
                    Logger::log(ERROR, "[base.dict]: get method expects a string as first atom.");
                }
            }
            else if (utils::stringsMatch(inlet, "set"))
            {
                if (message.indexMatchesType(0, 's'))
                {
                    Message value = message.cloneRange(1, message.getSize());
                    dict_.set(message.getString(0).c_str(), value);
                }
                else
                {
                    Logger::log(ERROR, "[base.dict]: set method expects a string as first atom.");
                }
            }
            else if (utils::stringsMatch(inlet, "remove"))
            {
                if (message.indexMatchesType(0, 's'))
                {
                    dict_.remove(message.getString(0).c_str());
                }
                else
                {
                    Logger::log(ERROR, "[base.dict]: set method expects a string as first atom.");
                }
            }
            else if (utils::stringsMatch(inlet, "list"))
            {
                std::vector<std::string> keys = dict_.listKeys();
                Message ret;
                std::vector<std::string>::const_iterator iter;
                for (iter = keys.begin(); iter != keys.end(); ++iter)
                {
                    ret.appendString((*iter).c_str());
                }
                output("list", ret);
            }
            else if (utils::stringsMatch(inlet, "reset"))
            {
                dict_.reset();
            }
        }
};

} // end of namespace
} // end of namespace
} // end of namespace

#endif // ifndef


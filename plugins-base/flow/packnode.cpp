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
#include "plugins-base/flow/packnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const PackNode::RESULT_OUTLET = "result";
const char * const PackNode::NUM_ATTR = "number";
const char * const PackNode::ALL_HOT_ATTR = "all-hot";

PackNode::PackNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute(NUM_ATTR, Message("i", 0), "Number of atoms to pack together. The object creates as many inlets numbered from 0 to n-1.")));
    addAttribute(Attribute::ptr(new Attribute(ALL_HOT_ATTR, Message("b", false), "If true, makes all inlets hot. (output the results when a message is received.")));
    Logger::log(DEBUG, "[pack] constructor:  = (i:0");
    addOutlet(RESULT_OUTLET, "Resulting packed message goes here.");
    setShortDocumentation("Combines several atoms into one message.");
}

void PackNode::processMessage(const char *inlet, const Message &message)
{
    bool is_a_packing_inlet = false;
    int current_number_of_inlets =
        this->getAttributeValue(NUM_ATTR).getInt(0);
    bool all_hot = this->getAttributeValue(ALL_HOT_ATTR).getBoolean(0);
    for (int i = 0; i < current_number_of_inlets; ++i)
    {
        std::ostringstream name_os;
        name_os << i;
        if (utils::stringsMatch(inlet, name_os.str().c_str()))
        {
            is_a_packing_inlet = true;
            stored_messages_[i] = message;
            if (i == 0 || all_hot)
                this->outputResult();
            break;
        }
    }
}

void PackNode::outputResult()
{
    Message ret;
    int current_number_of_inlets =
        this->getAttributeValue(NUM_ATTR).getInt(0);
    for (int i = current_number_of_inlets - 1; i >= 0; --i)
    {
        std::ostringstream name_os;
        name_os << i;
        unsigned int index = (unsigned int) i; // XXX

        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "[pack] " << __FUNCTION__ << ": add atom to output: " << i;
            Logger::log(INFO, os);
        }
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "[pack] " << __FUNCTION__ << ": add atom to output: " << i << " " << stored_messages_[index];
            Logger::log(INFO, os);
        }
        ret.prependMessage(stored_messages_[index]);
        // TODO: implement appendMessage
    }
    output(RESULT_OUTLET, ret);
}

void PackNode::printInletsInfo() const
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[pack]: actual inlets:";
        std::vector<std::string> inlets = this->listInlets();
        std::vector<std::string>::const_iterator iter;
        for (iter = inlets.begin(); iter != inlets.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os.str().c_str());
    }
}

bool PackNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (! utils::stringsMatch(NUM_ATTR, name))
        return true; // true means it's OK to change that attribute
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[pack] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
        Logger::log(DEBUG, os);
    }
    this->printInletsInfo();

    int new_size = value.getInt(0);
    if (new_size < 0)
    {
        std::ostringstream os;
        os << "[pack] " << __FUNCTION__ << ": Could not change attr to a negative value. name=\"" << name << "\" value=" << value;
        Logger::log(ERROR, os);
        return false;
    }
    int current_number_of_inlets =
        this->getAttributeValue(NUM_ATTR).getInt(0);
    if (new_size > current_number_of_inlets)
    {
        Message empty;
        for (int i = current_number_of_inlets; i < new_size; ++i)
        {
            std::ostringstream name_os;
            name_os << i;
            std::ostringstream doc;
            doc << "Input for the " << i << "th message to pack together with the other ones.";
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "[pack] " << __FUNCTION__ << ": add inlet " << i;
                Logger::log(INFO, os);
            }
            addInlet(name_os.str().c_str(), doc.str().c_str());
            stored_messages_.push_back(empty);
        }
    }
    else if (new_size < current_number_of_inlets)
    {
        for (int i = new_size; i < current_number_of_inlets; ++i)
        {
            std::ostringstream name_os;
            name_os << i;
            {
                // std::ostringstream os;
                // os << "[pack] " << __FUNCTION__ << ": remove inlet " << i;
                // Logger::log(INFO, os);
            }
            removeInlet(name_os.str().c_str());
        }
        stored_messages_.erase(stored_messages_.begin() + new_size, stored_messages_.end());
    }
    this->printInletsInfo();
    return true;
}

} // end of namespace
} // end of namespace


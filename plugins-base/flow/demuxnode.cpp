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
#include "plugins-base/flow/demuxnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <boost/lexical_cast.hpp>

namespace tempi {
namespace plugins_base {

const char * const DemuxNode::SELECTED_OUTLET_ATTR = "selected";
const char * const DemuxNode::NUM_OUTLETS_ATTR = "num_choices";
const char * const DemuxNode::INPUT_INLET = "input";
const char * const DemuxNode::SELECT_OUTLET_INLET = "choose";

DemuxNode::DemuxNode() :
    Node()
{
    this->setShortDocumentation("Lets messages flow through one of its outlet according to the selected index.");
    this->addAttribute(Attribute::ptr(new Attribute(SELECTED_OUTLET_ATTR, Message("i", -1), "Index of the outlet to output messages from. Use -1 or an invalid number to disable output completely.", "Indices go from 0 to n-1")));
    this->addAttribute(Attribute::ptr(new Attribute(NUM_OUTLETS_ATTR, Message("i", 0), "Number of outlets to have.")));
    this->addInlet(INPUT_INLET, "Incoming messages.");
    this->addInlet(SELECT_OUTLET_INLET, "Convenient way to set the selected outlet.", "Same as setting the attribute.", "i");
}

static std::string intToString(int i)
{
    return boost::lexical_cast<std::string>(i);
}

void DemuxNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(INPUT_INLET, inlet))
    {
        int selected = this->getAttributeValue(SELECTED_OUTLET_ATTR).getInt(0);
        if (selected < 0)
        {
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "DemuxNode: Discard message since selected < 0";
                Logger::log(INFO, os);
            }
            return;
        }
        if (selected < this->getAttributeValue(NUM_OUTLETS_ATTR).getInt(0))
        {
            std::string outlet_name = intToString(selected);
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "DemuxNode: Output via " << outlet_name;
                Logger::log(INFO, os);
            }
            output(outlet_name.c_str(), message);
        }
    }
    else if (utils::stringsMatch(SELECT_OUTLET_INLET, inlet))
    {
        if (Logger::isEnabledFor(NOTICE))
        {
            std::ostringstream os;
            os << "DemuxNode: choose " << message.getInt(0);
            Logger::log(NOTICE, os);
        }
        this->setAttributeValue(SELECTED_OUTLET_ATTR, message);
    }
}

bool DemuxNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(SELECTED_OUTLET_ATTR, name))
        return true;
    else if (utils::stringsMatch(NUM_OUTLETS_ATTR, name))
    {
        int before = this->getAttributeValue(NUM_OUTLETS_ATTR).getInt(0);
        int after = value.getInt(0);
        this->changeNumberOfOutlets(before, after);
        return true;
    }
    else
        return true;
}

void DemuxNode::changeNumberOfOutlets(int before, int after)
{
    // make sure they are positive
    if (after < 0)
        after = 0;
    if (before < 0)
        before = 0;

    if (after == before)
    {
        return;
    }
    if (after < before) // must delete some outlets
    {
        for (int i = after; i < before; i++)
        {
            std::string name = intToString(i);
            if (Logger::isEnabledFor(NOTICE))
            {
                std::ostringstream os;
                os << "DemuxNode: removeOutlet " << i;
                Logger::log(NOTICE, os);
            }
            this->removeOutlet(name.c_str());
        }
    }
    else if (after > before) // must create some outlets
    {
        for (int i = before; i < after; i++)
        {
            std::string name = intToString(i);
            if (Logger::isEnabledFor(NOTICE))
            {
                std::ostringstream os2;
                os2 << "DemuxNode: addOutlet " << i;
                Logger::log(NOTICE, os2);
            }
            std::ostringstream os;
            os << "Messages flow though this outlet if the chosen one is index " << i;
            this->addOutlet(name.c_str(), os.str().c_str());
        }
    }
}

} // end of namespace
} // end of namespace


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

#include "plugins-base/sampler/samplenode.h"
#include "tempi/message.h"
#include "tempi/utils.h"
#include "tempi/scheduler.h"
#include "tempi/graph.h"
#include "tempi/log.h"
#include <vector>

namespace tempi {
namespace sampler {

const char * const SamplerSampleNode::ATTR_NAME = "name";

SamplerSampleNode::SamplerSampleNode() :
    Node()
{
    setShortDocumentation("Manages one Region that is accessible by all nodes.");
    addAttribute(Attribute::ptr(new Attribute(ATTR_NAME, Message("s", ""), "Name of the region to create. An empty string means it is invalid.")));
}

void SamplerSampleNode::onAttributeChanged(const char *name, const Message &value)
{
    const static std::string region(ATTR_NAME);
    if (region == name)
    {
        if (value.getString(0) == previous_region_name_)
        {
            std::ostringstream os;
            os << "FIXME: attributes are set twice.";
            Logger::log(INFO, os.str().c_str());
            return;
        }
        bool ok = setRegionName(value.getString(0));
        if (! ok)
        {
            std::ostringstream os;
            os << "SamplerSampleNode." << __FUNCTION__ << ": " <<
                "Could not set region name to " << name << ". Will fallback to an empty string.";
            Logger::log(ERROR, os.str().c_str());
            this->setAttributeValue(ATTR_NAME, Message("s", ""));
            previous_region_name_ = "";
        }
    }
}

bool SamplerSampleNode::setRegionName(const std::string &name)
{
    Graph *graph = this->getGraph();
    if (graph == 0)
    {
        Logger::log(ERROR, "SamplerSampleNode.setRegionName: Graph is NULL");
        return false;
    }
    Scheduler *scheduler = graph->getScheduler();
    if (scheduler == 0)
    {
        Logger::log(ERROR, "SamplerSampleNode.setRegionName: Scheduler is NULL");
        return false;
    }
    if (scheduler->hasRegion(name.c_str()))
    {
        std::ostringstream os;
        os << "SamplerSampleNode.setRegionName: Already has region: " << name;
        Logger::log(ERROR, os.str().c_str());
        return false;
    }
    if (previous_region_name_ != "")
        scheduler->removeRegion(previous_region_name_.c_str());
    scheduler->createRegion(name.c_str());
    previous_region_name_ = name;
    return true;
}

void SamplerSampleNode::processMessage(const char *inlet, const Message &message)
{
    // pass
}

} // end of namespace
} // end of namespace


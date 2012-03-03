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

#include "plugins-base/sampler/samplerreadnode.h"
#include "tempi/message.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include "tempi/scheduler.h"
#include "tempi/graph.h"
#include <vector>

namespace tempi {
namespace plugins_base {

const char * const SamplerReadNode::ATTR_READING = "reading";
const char * const SamplerReadNode::ATTR_REGION = "region";
const char * const SamplerReadNode::OUTLET_READ = "read";

SamplerReadNode::SamplerReadNode() :
    Node(),
    empty_region_(new sampler::Region()),
    player_(new sampler::Player(empty_region_))
{
    setShortDocumentation("Reads sampled messages from a Region.");
    addAttribute(Attribute::ptr(new Attribute(ATTR_READING, Message("b", false), "Currently playing or not.")));
    addAttribute(Attribute::ptr(new Attribute(ATTR_REGION, Message("s", ""), "Name of the region to read from.")));

    addOutlet(OUTLET_READ, "Played back messages.");
}

void SamplerReadNode::onAttributeChanged(const char *name, const Message &value)
{
    const static std::string playing(ATTR_READING);
    const static std::string region(ATTR_REGION);

    if (playing == name)
        play(value.getBoolean(0));
    if (region == name)
        setRegion(value.getString(0));
}

void SamplerReadNode::play(bool enabled)
{
    if (enabled)
    {
        player_->reset();
    }
}

void SamplerReadNode::setRegion(const std::string &name)
{
    if (name == "")
    {
        Logger::log(INFO, "SamplerReadNode.setRegion: name is an empty string");
        return;
    }
    Graph *graph = this->getGraph();
    if (graph == 0)
    {
        Logger::log(ERROR, "SamplerReadNode.setRegion: Graph is NULL");
        return;
    }
    Scheduler *scheduler = graph->getScheduler();
    if (scheduler == 0)
    {
        Logger::log(ERROR, "SamplerReadNode.setRegion: Scheduler is NULL");
        return;
    }
    if (! scheduler->hasRegion(name.c_str()))
    {
        std::ostringstream os;
        os << "SamplerReadNode.setRegion: No such region: " << name;
        Logger::log(ERROR, os.str().c_str());
        return;
    }
    player_->setRegion(scheduler->getRegion(name.c_str()));
}

void SamplerReadNode::doTick()
{
    if (getAttributeValue(ATTR_READING).getBoolean(0))
    {
        std::vector<Message> messages;
        bool ok = player_->read(messages);
        if (ok)
        {
            std::vector<Message>::const_iterator iter;
            for (iter = messages.begin(); iter != messages.end(); ++iter)
                output(OUTLET_READ, (*iter));
        }
    }
}

void SamplerReadNode::processMessage(const char *inlet, const Message &message)
{
    // pass
}

} // end of namespace
} // end of namespace


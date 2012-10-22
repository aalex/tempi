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

#include "plugins-base/sampler/samplerwritenode.h"
#include "tempi/message.h"
#include "tempi/utils.h"
#include "tempi/scheduler.h"
#include "tempi/graph.h"
#include "tempi/log.h"
#include <vector>

namespace tempi {
namespace plugins_base {

const char * const SamplerWriteNode::ATTR_WRITING = "writing";
const char * const SamplerWriteNode::ATTR_REGION = "region";
const char * const SamplerWriteNode::INLET_WRITE = "write";

SamplerWriteNode::SamplerWriteNode() :
    Node(),
    empty_region_(new sampler::Region()),
    recorder_(new sampler::Recorder(empty_region_))
{
    setShortDocumentation("Writes sampled messages to a Region.");
    addAttribute(Attribute::ptr(new Attribute(ATTR_WRITING, Message("b", false), "Currently writing or not.")));
    addAttribute(Attribute::ptr(new Attribute(ATTR_REGION, Message("s", ""), "Name of the region to write to.")));

    addInlet(INLET_WRITE, "Messages to write.");
}

bool SamplerWriteNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    const static std::string writing(ATTR_WRITING);
    const static std::string region(ATTR_REGION);

    if (writing == name)
        record(value.getBoolean(0));
    if (region == name)
        setRegion(value.getString(0));
    return true;
}

void SamplerWriteNode::record(bool enabled)
{
    if (enabled)
    {
        recorder_->start(); // at beginning
    }
    else
    {
        recorder_->stop();
    }
}

void SamplerWriteNode::setRegion(const std::string &name)
{
    Graph *graph = this->getGraph();
    if (graph == 0)
    {
        Logger::log(ERROR, "SamplerWriteNode.setRegion: Graph is NULL");
        return;
    }
    Scheduler *scheduler = graph->getScheduler();
    if (scheduler == 0)
    {
        Logger::log(ERROR, "SamplerWriteNode.setRegion: Scheduler is NULL");
        return;
    }
    if (! scheduler->hasRegion(name.c_str()))
    {
        std::ostringstream os;
        os << "SamplerWriteNode.setRegion: No such region: " << name;
        Logger::log(ERROR, os.str().c_str());
        return;
    }
    recorder_->setRegion(scheduler->getRegion(name.c_str()));
}

void SamplerWriteNode::doTick()
{
    // pass
}

void SamplerWriteNode::processMessage(const char *inlet, const Message &message)
{
    bool rec = getAttributeValue(ATTR_WRITING).getBoolean(0);
    if (utils::stringsMatch(inlet, INLET_WRITE))
    {
        if (rec)
        {
            recorder_->add(message);
        }
    }
}

} // end of namespace
} // end of namespace


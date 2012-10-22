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
 * The SamplerWriteNode class.
 */

#ifndef __TEMPI_SAMPLER_WRITE_NODE_H__
#define __TEMPI_SAMPLER_WRITE_NODE_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include <string>
#include "tempi/sampler_score.h"
#include "tempi/timeposition.h"
#include "tempi/identifier.h"
#include "tempi/sampler_region.h"
#include "tempi/sampler_recorder.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Writes Messages into a Region.
 */
class SamplerWriteNode : public Node
{
    public:
        SamplerWriteNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        sampler::Region::ptr empty_region_;
        sampler::Recorder::ptr recorder_;
        void record(bool enabled);
        virtual void doTick();
        void setRegion(const std::string &name);
        static const char * const ATTR_WRITING;
        static const char * const ATTR_REGION;
        static const char * const INLET_WRITE;
};

} // end of namespace
} // end of namespace

#endif // ifndef


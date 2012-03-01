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
 * The SamplerSampleNode class.
 */

#ifndef __TEMPI_SAMPLER_SAMPLE_NODE_H__
#define __TEMPI_SAMPLER_SAMPLE_NODE_H__

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
namespace sampler {

/**
 * Creates a Region accessible by all nodes within the same scheduler.
 */
class SamplerSampleNode : public Node
{
    public:
        SamplerSampleNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void onAttributeChanged(const char *name, const Message &value);
    private:
        std::string previous_region_name_;
        static const char * const ATTR_NAME;
        bool setRegionName(const std::string &name);
};

} // end of namespace
} // end of namespace

#endif // ifndef


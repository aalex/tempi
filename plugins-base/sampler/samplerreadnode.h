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
 * The SamplerReadNode class.
 */

#ifndef __TEMPI_SAMPLER_READ_NODE_H__
#define __TEMPI_SAMPLER_READ_NODE_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include <string>
#include "tempi/sampler_score.h"
#include "tempi/timeposition.h"
#include "tempi/identifier.h"
#include "tempi/sampler_region.h"
#include "tempi/sampler_player.h"
#include "tempi/node.h"

namespace tempi {
namespace sampler {

/**
 * Reads Regions.
 */
class SamplerReadNode : public Node
{
    public:
        SamplerReadNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void onAttributeChanged(const char *name, const Message &value);
    private:
        Region::ptr empty_region_;
        Player::ptr player_;
        void play(bool enabled);
        virtual void doTick();
        void setRegion(const std::string &name);
        static const char * const ATTR_READING;
        static const char * const ATTR_REGION;
        static const char * const OUTLET_READ;
};

} // end of namespace
} // end of namespace

#endif // ifndef


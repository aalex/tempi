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
 * @file
 * The MapperInputNode class.
 */
#ifndef __TEMPI_MAPPER_MAPPERINPUTNODE_H__
#define __TEMPI_MAPPER_MAPPERINPUTNODE_H__

#include "tempi/config.h"
#ifdef HAVE_LIBMAPPER

#include "tempi/node.h"
#include "tempi/message.h"
#include "tempi/concurrentqueue.h"
#include "plugins-base/mapper/mapperdevice.h"

namespace tempi {
namespace plugins_base {

/**
 * Node that receives libmapper messages.
 */
class MapperInputNode : public Node
{
    public:
        MapperInputNode();
    protected:
        virtual void doTick();
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        MapperDevice device_;
        //unsigned int port_number_;
        //std::string input_name;
};

} // end of namespace
} // end of namespace

#endif // HAVE_LIBMAPPER
#endif // ifndef


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
 * The Filter class.
 */

#ifndef __TEMPI_MAPPING_FILTER_H__
#define __TEMPI_MAPPING_FILTER_H__

#include "tempi/node.h"
#include "tempi/sharedptr.h"
#include "tempi/outlet.h"
#include "tempi/inlet.h"

namespace tempi {
namespace plugins_base {

/**
 * A Filter is Node who has a sink and source and usually filters data.
 */
class Filter : public Node
{
    public:
        Filter();
        virtual ~Filter() {}
    private:
        // Inherited from Node:
        virtual void processMessage(const char *name, const Message &message);
        // New virtual method:
        virtual Message filter(const Message &message) = 0;
};

} // end of namespace
} // end of namespace

#endif // ifndef


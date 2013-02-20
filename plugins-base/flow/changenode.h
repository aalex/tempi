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
 * The ChangeNode class.
 */
#ifndef __TEMPI_BASE_FLOW_CHANGE_NODE_H__
#define __TEMPI_BASE_FLOW_CHANGE_NODE_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * The ChangeNode outputs messages only if its different from the last one it received.
 */
class ChangeNode : public Node
{
    public:
        static const char * const IN_INLET;
        static const char * const OUT_OUTLET;
        ChangeNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        Message last_;
};

} // end of namespace
} // end of namespace

#endif // ifndef


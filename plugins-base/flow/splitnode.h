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
 * The SplitNode class.
 */
#ifndef __TEMPI_BASE_SPLIT_NODE_H__
#define __TEMPI_BASE_SPLIT_NODE_H__

#include "tempi/node.h"
#include <vector>
#include <string>

namespace tempi {
namespace plugins_base {

/**
 * Splits a message into atoms.
 */
class SplitNode : public Node
{
    public:
        SplitNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        static const char * const INPUT_INLET;
        static const char * const FIRST_N_OUTLET;
        static const char * const LEFTOVER_OUTLET;
        static const char * const TOOSHORT_OUTLET;
        static const char * const N_ATTR;
};

} // end of namespace
} // end of namespace

#endif // ifndef


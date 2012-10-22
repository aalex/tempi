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
 * The StringSplitNode class.
 */
#ifndef __TEMPI_STRING_SPLIT_NODE_H__
#define __TEMPI_STRING_SPLIT_NODE_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Splits a string into characters.
 */
class StringSplitNode : public Node
{
    public:
        StringSplitNode();
    //protected:
    //    virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        virtual void processMessage(const char *inlet, const Message &message);
        static const char * const STRING_INLET;
        static const char * const TOKENS_OUTLET;
        static const char * const SEPARATOR_ATTR;
};

} // end of namespace
} // end of namespace

#endif // ifndef


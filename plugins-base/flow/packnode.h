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
 * The PackNode class.
 */
#ifndef __TEMPI_BASE_PACK_NODE_H__
#define __TEMPI_BASE_PACK_NODE_H__

#include "tempi/node.h"
#include <vector>
#include <string>

namespace tempi {
namespace plugins_base {

/**
 * Combines several atoms into a message.
 */
class PackNode : public Node
{
    public:
        PackNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        void printInletsInfo() const;
        std::vector<Message> stored_messages_;
        void outputResult();
        static const char * const RESULT_OUTLET;
        static const char * const NUM_ATTR;
        static const char * const ALL_HOT_ATTR;
};

} // end of namespace
} // end of namespace

#endif // ifndef


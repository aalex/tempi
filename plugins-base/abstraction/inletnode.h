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
 * The InletNode class.
 */
#ifndef __TEMPI_ABSTRACTION_INLETNODE_H__
#define __TEMPI_ABSTRACTION_INLETNODE_H__

#include "tempi/node.h"
#include "tempi/concurrentqueue.h"

namespace tempi {
namespace plugins_base {

/**
 * Inlet in an abstraction
 */
class InletNode : public Node
{
    public:
        InletNode();
        void push(const Message &message);
    protected:
        virtual void doTick();
        virtual void processMessage(const char *inlet, const Message &message)
        {}
    private:
        ConcurrentQueue<Message> queue_;
};

} // end of namespace
} // end of namespace

#endif // ifndef


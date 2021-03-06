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
 * The MetroNode class.
 */

#ifndef __TEMPI_METRONODE_H__
#define __TEMPI_METRONODE_H__

#include "tempi/timer.h"
#include "tempi/timeposition.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * A MetroNode is a Node that ticks every interval ms.
 */
class MetroNode : public Node
{
    public:
        MetroNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message) {}
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        bool initial_tick_done_;
        Timer timer_;
        TimePosition interval_;

        virtual void doTick();
        void outputTick();
        void startMetro();
};

} // end of namespace
} // end of namespace

#endif // ifndef


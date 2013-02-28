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
 * The DemuxNode class.
 */
#ifndef __TEMPI_BASE_DEMUXNODE_H__
#define __TEMPI_BASE_DEMUXNODE_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * The DemuxNode lets messages flow through one of its outlets according to the selected index.
 */
class DemuxNode : public Node
{
    public:
        DemuxNode();
        static const char * const SELECTED_OUTLET_ATTR;
        static const char * const NUM_OUTLETS_ATTR;
        static const char * const INPUT_INLET;
        static const char * const SELECT_OUTLET_INLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        void changeNumberOfOutlets(int before, int after);
};

} // end of namespace
} // end of namespace

#endif // ifndef


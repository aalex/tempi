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
 * The GraphEditorNode class.
 */

#ifndef __TEMPI_BASE_GRAPHEDITORNODE_H__
#define __TEMPI_BASE_GRAPHEDITORNODE_H__


#include "tempi/utils.h"
#include "tempi/node.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

/**
 * The GraphEditorNode edits the current graph.
 */
class GraphEditorNode : public Node
{
    public:
        GraphEditorNode();
    private:
        virtual void processMessage(const char *inlet, const Message &message);
        static const char * const COMMAND_INLET;
        static const char * const SUCCESS_OUTLET;
};

} // end of namespace
} // end of namespace

#endif // ifndef


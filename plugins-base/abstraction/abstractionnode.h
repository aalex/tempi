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
 * The OscReceiverNode class.
 */
#ifndef __TEMPI_PLUGINS_BASE_ABSTRACTION_NODE_H__
#define __TEMPI_PLUGINS_BASE_ABSTRACTION_NODE_H__

#include "tempi/node.h"
#include "tempi/synchronousscheduler.h"
#include <string>
#include <vector>

namespace tempi {
namespace plugins_base {

/**
 * Loads a Graph from an XML file.
 */
class AbstractionNode : public Node
{
    public:
        AbstractionNode();
    protected:
        virtual void doTick();
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        std::string file_path_;
        tempi::SynchronousScheduler::ptr scheduler_;
        tempi::Graph::ptr graph_;
        std::vector<std::string> outlet_names_;
        std::vector<std::string> inlet_names_;
        bool loadGraph();
        bool deleteGraph();
        bool loadInletsAndOutlets();
        static const char * const INLET_NODE_NAME;
        static const char * const OUTLET_NODE_NAME;
};

} // end of namespace
} // end of namespace

#endif // ifndef


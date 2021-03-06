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
 * The LoggerNode class.
 */
#ifndef __TEMPI_BASE_LOGGER_NODE_H__
#define __TEMPI_BASE_LOGGER_NODE_H__

#include "tempi/node.h"
#include "tempi/message.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

/**
 * Logs the contents of a Message using Tempi's logging system.
 */
class LoggerNode : public Node
{
    public:
        LoggerNode();
        static const char * const LEVEL_ATTR;
        static const char * const LOG_INLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
};

} // end of namespace
} // end of namespace

#endif // ifndef


/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
 * The RouteNode class.
 */
#ifndef __TEMPI_BASE_ROUTENODE_H__
#define __TEMPI_BASE_ROUTENODE_H__

#include "tempi/node.h"
#include <vector>
#include <string>

namespace tempi {
namespace plugins_base {

/**
 * The RouteNode routes messages to its different outlets according to the first string in each message.
 */
class RouteNode : public Node
{
    public:
        RouteNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        std::vector<std::string> selectors_;
        void printOutletsInfo() const;
};

} // end of namespace
} // end of namespace

#endif // ifndef


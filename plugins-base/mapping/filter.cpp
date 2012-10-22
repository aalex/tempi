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

#include "plugins-base/mapping/filter.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

Filter::Filter() :
    Node()
{
    addInlet("0");
    addOutlet("0");
}

void Filter::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "0"))
    {
        getOutlet("0")->trigger(filter(message));
    }
}

} // end of namespace
} // end of namespace


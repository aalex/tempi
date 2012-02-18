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

#include "tempi/base/anynode.h"
#include "tempi/base/appendnode.h"
#include "tempi/base/appsinknode.h"
#include "tempi/base/baselibrary.h"
#include "tempi/base/counternode.h"
#include "tempi/base/delaynode.h"
#include "tempi/base/metro_node.h"
#include "tempi/base/nop_node.h"
#include "tempi/base/print_node.h"
#include "tempi/base/spigotnode.h"
#include "tempi/base/prependnode.h"
#include "tempi/base/routenode.h"
#include "tempi/base/castnode.h"
#include "tempi/base/loadmessnode.h"
#include "tempi/base/spatosc/spatoscnode.h"
#include "tempi/utils.h"
#include "tempi/config.h"

namespace tempi {
namespace base {

void BaseLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;
    factory.registerTypeT<PrintNode>(concatenate(prefix, "print").c_str());
    factory.registerTypeT<AppendNode>(concatenate(prefix, "append").c_str());
    factory.registerTypeT<NopNode>(concatenate(prefix, "nop").c_str());
    factory.registerTypeT<MetroNode>(concatenate(prefix, "metro").c_str());
    factory.registerTypeT<AnyNode>(concatenate(prefix, "any").c_str());
    factory.registerTypeT<CounterNode>(concatenate(prefix, "counter").c_str());
    factory.registerTypeT<DelayNode>(concatenate(prefix, "delay").c_str());
    factory.registerTypeT<AppSinkNode>(concatenate(prefix, "appsink").c_str());
    factory.registerTypeT<SpigotNode>(concatenate(prefix, "spigot").c_str());
    factory.registerTypeT<PrependNode>(concatenate(prefix, "prepend").c_str());
    factory.registerTypeT<RouteNode>(concatenate(prefix, "route").c_str());
    factory.registerTypeT<LoadMessNode>(concatenate(prefix, "loadmess").c_str());
    factory.registerTypeT<CastNode>(concatenate(prefix, "cast").c_str());
#ifdef HAVE_SPATOSC
    factory.registerTypeT<tempi::base::SpatoscNode>(concatenate(prefix, "spatosc").c_str());
#endif // HAVE_SPATOSC
}

} // end of namespace
} // end of namespace


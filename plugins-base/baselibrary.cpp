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

#include "plugins-base/baselibrary.h"
#include "plugins-base/flow/anynode.h"
#include "plugins-base/flow/appendnode.h"
#include "plugins-base/flow/appsinknode.h"
#include "plugins-base/flow/castnode.h"
#include "plugins-base/flow/counternode.h"
#include "plugins-base/flow/delaynode.h"
#include "plugins-base/flow/loadmessnode.h"
#include "plugins-base/flow/metro_node.h"
#include "plugins-base/flow/nop_node.h"
#include "plugins-base/flow/prependnode.h"
#include "plugins-base/flow/print_node.h"
#include "plugins-base/flow/routenode.h"
#include "plugins-base/spatosc/spatoscnode.h"
#include "plugins-base/flow/spigotnode.h"
#include "plugins-base/clutter/clutterstagenode.h"
#include "plugins-base/math/mathnodes.h"
#include "plugins-base/midi/midireceivernode.h"
#include "plugins-base/midi/midiroutenode.h"
#include "plugins-base/midi/midisendernode.h"
#include "plugins-base/osc/oscreceivernode.h"
#include "plugins-base/osc/oscsendernode.h"
#include "plugins-base/sampler/samplernode.h"
#include "tempi/config.h"
#include "tempi/nodefactory.h"
#include "tempi/utils.h"

namespace tempi {
namespace base {

void BaseLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;
    using namespace tempi::clutter;
    using namespace tempi::osc;
    using namespace tempi::midi;
    using namespace tempi::base;
    using namespace tempi::math;
    using namespace tempi::sampler;

    factory.registerTypeT<PrintNode>(concatenate("base.", "print").c_str());
    factory.registerTypeT<AppendNode>(concatenate("base.", "append").c_str());
    factory.registerTypeT<NopNode>(concatenate("base.", "nop").c_str());
    factory.registerTypeT<MetroNode>(concatenate("base.", "metro").c_str());
    factory.registerTypeT<AnyNode>(concatenate("base.", "any").c_str());
    factory.registerTypeT<CounterNode>(concatenate("base.", "counter").c_str());
    factory.registerTypeT<DelayNode>(concatenate("base.", "delay").c_str());
    factory.registerTypeT<AppSinkNode>(concatenate("base.", "appsink").c_str());
    factory.registerTypeT<SpigotNode>(concatenate("base.", "spigot").c_str());
    factory.registerTypeT<PrependNode>(concatenate("base.", "prepend").c_str());
    factory.registerTypeT<RouteNode>(concatenate("base.", "route").c_str());
    factory.registerTypeT<LoadMessNode>(concatenate("base.", "loadmess").c_str());
    factory.registerTypeT<CastNode>(concatenate("base.", "cast").c_str());
#ifdef HAVE_SPATOSC
    factory.registerTypeT<SpatoscNode>(concatenate("base.", "spatosc").c_str());
#endif // HAVE_SPATOSC
    factory.registerTypeT<AddNode>(concatenate("math.", "+").c_str());
    factory.registerTypeT<DivNode>(concatenate("math.", "/").c_str());
    factory.registerTypeT<EqualsNotNode>(concatenate("math.", "!=").c_str());
    factory.registerTypeT<IsEqualNode>(concatenate("math.", "==").c_str());
    factory.registerTypeT<IsGreaterNode>(concatenate("math.", ">").c_str());
    factory.registerTypeT<IsLessNode>(concatenate("math.", "<").c_str());
    factory.registerTypeT<SubtractNode>(concatenate("math.", "-").c_str());
    factory.registerTypeT<MultNode>(concatenate("math.", "*").c_str());
    factory.registerTypeT<DegToRadNode>(concatenate("math.", "deg2rad").c_str());
#ifdef HAVE_CLUTTER
    factory.registerTypeT<TempiClutterStageNode>(concatenate("clutter.", "stage").c_str());
#endif // HAVE_CLUTTER
    factory.registerTypeT<MidiReceiverNode>(concatenate("midi.", "receive").c_str());
    factory.registerTypeT<MidiSenderNode>(concatenate("midi.", "send").c_str());
    factory.registerTypeT<MidiRouteNode>(concatenate("midi.", "route").c_str());
// TODO #ifdef HAVE_LIBLO
    factory.registerTypeT<OscReceiverNode>(concatenate("osc.", "receive").c_str());
    factory.registerTypeT<OscSenderNode>(concatenate("osc.", "send").c_str());
// TODO #endif // HAVE_SPATOSC
    factory.registerTypeT<SamplerNode>(concatenate("sampler.", "sampler").c_str());
}

} // end of namespace
} // end of namespace

extern "C"

void tempi_base_setup(void *data)
{
    tempi::NodeFactory *factory = static_cast<tempi::NodeFactory*>(data);
    tempi::base::BaseLibrary lib;
    lib.load(*factory, "UNUSED");
}


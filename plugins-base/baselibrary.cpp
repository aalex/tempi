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
#include "plugins-base/flow/dictnode.h"
#include "plugins-base/flow/linenode.h"
#include "plugins-base/flow/loadmessnode.h"
#include "plugins-base/flow/metro_node.h"
#include "plugins-base/flow/nop_node.h"
#include "plugins-base/flow/packnode.h"
#include "plugins-base/flow/prependnode.h"
#include "plugins-base/flow/print_node.h"
#include "plugins-base/flow/routenode.h"
#include "plugins-base/flow/triggernode.h"
#include "plugins-base/flow/unpacknode.h"
#include "plugins-base/spatosc/spatoscnode.h"
#include "plugins-base/flow/spigotnode.h"
#include "plugins-base/clutter/clutterstagenode.h"
#include "plugins-base/math/onefloatmathnode.h"
#include "plugins-base/math/twofloatmathnode.h"
#include "plugins-base/math/booleanoperatornode.h"
#include "plugins-base/midi/midireceivernode.h"
#include "plugins-base/midi/midiroutenode.h"
#include "plugins-base/midi/midisendernode.h"
#include "plugins-base/music/musicnearestnode.h"
#include "plugins-base/music/monodicnode.h"
#include "plugins-base/osc/oscreceivernode.h"
#include "plugins-base/osc/oscsendernode.h"
#include "plugins-base/osc/oscprependnode.h"
#include "plugins-base/osc/oscroutenode.h"
#include "plugins-base/sampler/samplernode.h"
#include "plugins-base/sampler/samplenode.h"
#include "plugins-base/sampler/samplerwritenode.h"
#include "plugins-base/sampler/samplerreadnode.h"
#include "plugins-base/random/randomdrunknode.h"
#include "plugins-base/random/randomintnode.h"
#include "plugins-base/abstraction/abstractionnode.h"
#include "plugins-base/abstraction/outletnode.h"
#include "plugins-base/abstraction/inletnode.h"
#include "tempi/config.h"
#include "tempi/nodefactory.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

void BaseLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;

    factory.registerTypeT<PrintNode>(concatenate("base.", "print").c_str());
    factory.registerTypeT<AppendNode>(concatenate("base.", "append").c_str());
    factory.registerTypeT<NopNode>(concatenate("base.", "nop").c_str());
    factory.registerTypeT<PackNode>(concatenate("flow.", "pack").c_str());
    factory.registerTypeT<MetroNode>(concatenate("base.", "metro").c_str());
    factory.registerTypeT<AnyNode>(concatenate("base.", "any").c_str());
    factory.registerTypeT<CounterNode>(concatenate("base.", "counter").c_str());
    factory.registerTypeT<DelayNode>(concatenate("base.", "delay").c_str());
    factory.registerTypeT<DictNode>(concatenate("base.", "dict").c_str());
    factory.registerTypeT<AppSinkNode>(concatenate("base.", "appsink").c_str());
    factory.registerTypeT<SpigotNode>(concatenate("base.", "spigot").c_str());
    factory.registerTypeT<PrependNode>(concatenate("base.", "prepend").c_str());
    factory.registerTypeT<RouteNode>(concatenate("base.", "route").c_str());
    factory.registerTypeT<TriggerNode>(concatenate("flow.", "trigger").c_str());
    factory.registerTypeT<UnpackNode>(concatenate("flow.", "unpack").c_str());
    factory.registerTypeT<LineNode>(concatenate("flow.", "line").c_str());
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
    factory.registerTypeT<MapNode>(concatenate("math.", "map").c_str());
    factory.registerTypeT<MultNode>(concatenate("math.", "*").c_str());
    factory.registerTypeT<DegToRadNode>(concatenate("math.", "deg2rad").c_str());

#ifdef HAVE_CLUTTER
    factory.registerTypeT<TempiClutterStageNode>(concatenate("clutter.", "stage").c_str());
#endif // HAVE_CLUTTER

// TODO: #ifdef HAVE_STK
    factory.registerTypeT<MidiReceiverNode>(concatenate("midi.", "receive").c_str());
    factory.registerTypeT<MidiSenderNode>(concatenate("midi.", "send").c_str());
    factory.registerTypeT<MidiRouteNode>(concatenate("midi.", "route").c_str());
// TODO: #endif // HAVE_STK

    factory.registerTypeT<NearestNoteNode>(concatenate("music.", "nearest.note").c_str());
    factory.registerTypeT<MonodicNode>(concatenate("music.", "monodic").c_str());

// TODO #ifdef HAVE_LIBLO
    factory.registerTypeT<OscReceiverNode>(concatenate("osc.", "receive").c_str());
    factory.registerTypeT<OscSenderNode>(concatenate("osc.", "send").c_str());
// TODO #endif // HAVE_LIBLO
    factory.registerTypeT<OscPrependNode>(concatenate("osc.", "prepend").c_str());
    factory.registerTypeT<OscRouteNode>(concatenate("osc.", "route").c_str());

    factory.registerTypeT<SamplerSampleNode>(concatenate("sampler.", "sample").c_str());
    factory.registerTypeT<SamplerNode>(concatenate("sampler.", "sampler").c_str());
    factory.registerTypeT<SamplerReadNode>(concatenate("sampler.", "read").c_str());
    factory.registerTypeT<SamplerWriteNode>(concatenate("sampler.", "write").c_str());

#ifdef HAVE_GLIB
    factory.registerTypeT<RandomDrunkNode>(concatenate("random.", "drunk").c_str());
    factory.registerTypeT<RandomIntNode>(concatenate("random.", "int").c_str());
#endif // HAVE_GLIB

    factory.registerTypeT<AbstractionNode>(concatenate("abs.", "abstraction").c_str());
    factory.registerTypeT<OutletNode>(concatenate("abs.", "outlet").c_str());
    factory.registerTypeT<InletNode>(concatenate("abs.", "inlet").c_str());
}

} // end of namespace
} // end of namespace

extern "C"
void tempi_base_setup(void *data)
{
    tempi::NodeFactory *factory = static_cast<tempi::NodeFactory*>(data);
    tempi::plugins_base::BaseLibrary lib;
    lib.load(*factory, "UNUSED");
}


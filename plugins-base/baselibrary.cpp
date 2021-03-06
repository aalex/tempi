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

#include "plugins-base/3d/3dcomparenode.h"
#include "plugins-base/abstraction/abstractionnode.h"
#include "plugins-base/abstraction/inletnode.h"
#include "plugins-base/abstraction/outletnode.h"
#include "plugins-base/baselibrary.h"
#include "plugins-base/convert/booltointnode.h"
#include "plugins-base/edit/grapheditornode.h"
#include "plugins-base/flow/anynode.h"
#include "plugins-base/flow/appendnode.h"
#include "plugins-base/flow/appsinknode.h"
#include "plugins-base/flow/castnode.h"
#include "plugins-base/flow/changenode.h"
#include "plugins-base/flow/counternode.h"
#include "plugins-base/flow/demuxnode.h"
#include "plugins-base/flow/dictnode.h"
#include "plugins-base/flow/linenode.h"
#include "plugins-base/flow/loadmessnode.h"
#include "plugins-base/flow/logger_node.h"
#include "plugins-base/flow/nop_node.h"
#include "plugins-base/flow/packnode.h"
#include "plugins-base/flow/prependnode.h"
#include "plugins-base/flow/print_node.h"
#include "plugins-base/flow/routenode.h"
#include "plugins-base/flow/speedlimnode.h"
#include "plugins-base/flow/spigotnode.h"
#include "plugins-base/flow/splitnode.h"
#include "plugins-base/flow/triggernode.h"
#include "plugins-base/flow/typenode.h"
#include "plugins-base/flow/unpacknode.h"
#include "plugins-base/flow/send_and_receive_nodes.h"
#include "plugins-base/list/pick_indices.h"
#include "plugins-base/mapper/mapperinputnode.h"
#include "plugins-base/math/booleanoperatornode.h"
#include "plugins-base/math/jsexpr.h"
#include "plugins-base/math/luaexpr.h"
#include "plugins-base/math/onefloatmathnode.h"
#include "plugins-base/math/twofloatmathnode.h"
#include "plugins-base/midi/midibuildernodes.h"
#include "plugins-base/midi/midireceivernode.h"
#include "plugins-base/midi/midiroutenode.h"
#include "plugins-base/midi/midisendernode.h"
#include "plugins-base/music/monodicnode.h"
#include "plugins-base/music/musicnearestnode.h"
#include "plugins-base/os/shellnode.h"
#include "plugins-base/osc/oscprependnode.h"
#include "plugins-base/osc/oscreceivernode.h"
#include "plugins-base/osc/oscroutenode.h"
#include "plugins-base/osc/oscsendernode.h"
#include "plugins-base/random/randomdrunknode.h"
#include "plugins-base/random/randomintnode.h"
#include "plugins-base/sampler/samplenode.h"
#include "plugins-base/sampler/samplernode.h"
#include "plugins-base/sampler/samplerreadnode.h"
#include "plugins-base/sampler/samplerwritenode.h"
#include "plugins-base/serial/blobtobytes.h"
#include "plugins-base/serial/bytestoblobnode.h"
#include "plugins-base/serial/serialdevicenode.h"
#include "plugins-base/serial/todmxusbpro.h"
#include "plugins-base/spatosc/spatoscnode.h"
#include "plugins-base/string/stringcharactersnode.h"
#include "plugins-base/string/stringjoinnode.h"
#include "plugins-base/string/stringsplitnode.h"
#include "plugins-base/time/clock_node.h"
#include "plugins-base/time/timernode.h"
#include "plugins-base/time/cron_node.h"
#include "plugins-base/time/date_node.h"
#include "plugins-base/time/delaynode.h"
#include "plugins-base/time/metro_node.h"
#include "tempi/config.h"
#include "tempi/nodefactory.h"
#include "tempi/utils.h"

#ifdef HAVE_CLUTTER
#include "plugins-base/clutter/clutterstagenode.h"
#endif

namespace tempi {
namespace plugins_base {

void BaseLibrary::load(NodeFactory &factory, const char * /*prefix*/) const
{
    using utils::concatenate;

    static const char * const prefix = "base.";
    factory.registerTypeT<BoolToIntNode>(concatenate(prefix, "convert.booltoint").c_str());
    factory.registerTypeT<GraphEditorNode>(concatenate(prefix, "edit.grapheditor").c_str());
    factory.registerTypeT<PrintNode>(concatenate(prefix, "flow.print").c_str());
    factory.registerTypeT<AppendNode>(concatenate(prefix, "flow.append").c_str());
    factory.registerTypeT<NopNode>(concatenate(prefix, "flow.nop").c_str());
    factory.registerTypeT<PackNode>(concatenate(prefix, "flow.pack").c_str());
    factory.registerTypeT<MetroNode>(concatenate(prefix, "time.metro").c_str());
    factory.registerTypeT<ClockNode>(concatenate(prefix, "time.clock").c_str());
    factory.registerTypeT<DateNode>(concatenate(prefix, "time.date").c_str());
    factory.registerTypeT<CronNode>(concatenate(prefix, "time.cron").c_str());
    factory.registerTypeT<TimerNode>(concatenate(prefix, "time.timer").c_str());
    factory.registerTypeT<AnyNode>(concatenate(prefix, "flow.any").c_str());
    factory.registerTypeT<CounterNode>(concatenate(prefix, "flow.counter").c_str());
    factory.registerTypeT<DemuxNode>(concatenate(prefix, "flow.demux").c_str());
    factory.registerTypeT<DelayNode>(concatenate(prefix, "time.delay").c_str());
    factory.registerTypeT<DictNode>(concatenate(prefix, "data.dict").c_str());
    //factory.registerTypeT<AppSinkNode>(concatenate("base.", "data.appsink").c_str());
    factory.registerTypeT<SpigotNode>(concatenate(prefix, "flow.spigot").c_str());
    factory.registerTypeT<PrependNode>(concatenate(prefix, "flow.prepend").c_str());
    factory.registerTypeT<RouteNode>(concatenate(prefix, "flow.route").c_str());
    factory.registerTypeT<SpeedLimNode>(concatenate(prefix, "flow.speedlim").c_str());
    factory.registerTypeT<SplitNode>(concatenate(prefix, "flow.split").c_str());
    factory.registerTypeT<TriggerNode>(concatenate(prefix, "flow.trigger").c_str());
    factory.registerTypeT<TypeNode>(concatenate(prefix, "flow.type").c_str());
    factory.registerTypeT<UnpackNode>(concatenate(prefix, "flow.unpack").c_str());
    factory.registerTypeT<LineNode>(concatenate(prefix, "flow.line").c_str());
    factory.registerTypeT<LoadMessNode>(concatenate(prefix, "flow.loadmess").c_str());
    factory.registerTypeT<LoggerNode>(concatenate(prefix, "flow.logger").c_str());
    factory.registerTypeT<CastNode>(concatenate(prefix, "flow.cast").c_str());
    factory.registerTypeT<ChangeNode>(concatenate(prefix, "flow.change").c_str());

    factory.registerTypeT<PickIndicesNode>(concatenate(prefix, "list.pick_indices").c_str());
    factory.registerTypeT<SendNode>(concatenate(prefix, "flow.send").c_str());
    factory.registerTypeT<ReceiveNode>(ReceiveNode::NODE_TYPE_NAME); // We have to do this.

#ifdef HAVE_SPATOSC
    factory.registerTypeT<SpatoscNode>(concatenate(prefix, "osc.spatosc").c_str());
    factory.registerTypeT<Angle3dNode>(concatenate(prefix, "3d.angle").c_str());
#endif // HAVE_SPATOSC

#ifdef HAVE_V8
    factory.registerTypeT<JsExprNode>(concatenate(prefix, "math.jsexpr").c_str());
#endif // HAVE_V8
#ifdef HAVE_LUA
    factory.registerTypeT<LuaExprNode>(concatenate(prefix, "math.luaexpr").c_str());
#endif // HAVE_LUA

    factory.registerTypeT<AddNode>(concatenate(prefix, "math.+").c_str());
    factory.registerTypeT<CosNode>(concatenate(prefix, "math.cos").c_str());
    factory.registerTypeT<SinNode>(concatenate(prefix, "math.sin").c_str());
    factory.registerTypeT<RoundNode>(concatenate(prefix, "math.round").c_str());
    factory.registerTypeT<DivNode>(concatenate(prefix, "math./").c_str());
    factory.registerTypeT<EqualsNotNode>(concatenate(prefix, "math.!=").c_str());
    factory.registerTypeT<IsEqualNode>(concatenate(prefix, "math.==").c_str());
    factory.registerTypeT<IsGreaterNode>(concatenate(prefix, "math.>").c_str());
    factory.registerTypeT<IsGreaterOrEqualNode>(concatenate(prefix, "math.>=").c_str());
    factory.registerTypeT<IsLessNode>(concatenate(prefix, "math.<").c_str());
    factory.registerTypeT<IsLessOrEqualNode>(concatenate(prefix, "math.<=").c_str());
    factory.registerTypeT<SubtractNode>(concatenate(prefix, "math.-").c_str());
    factory.registerTypeT<MapNode>(concatenate(prefix, "math.map").c_str());
    factory.registerTypeT<MultNode>(concatenate(prefix, "math.*").c_str());
    factory.registerTypeT<ModuloNode>(concatenate(prefix, "math.%").c_str());
    factory.registerTypeT<DegToRadNode>(concatenate(prefix, "math.deg2rad").c_str());

#ifdef HAVE_CLUTTER
    factory.registerTypeT<TempiClutterStageNode>(concatenate(prefix, "clutter.stage").c_str());
#endif // HAVE_CLUTTER

// TODO: #ifdef HAVE_STK
    factory.registerTypeT<ControlBuilderNode>(concatenate(prefix, "midi.build.control").c_str());
    factory.registerTypeT<NoteBuilderNode>(concatenate(prefix, "midi.build.note").c_str());
    factory.registerTypeT<MidiReceiverNode>(concatenate(prefix, "midi.input").c_str());
    factory.registerTypeT<MidiSenderNode>(concatenate(prefix, "midi.output").c_str());
    factory.registerTypeT<MidiRouteNode>(concatenate(prefix, "midi.route").c_str());
    factory.registerTypeT<ProgramChangeBuilderNode>(concatenate(prefix, "midi.build.program").c_str());
// TODO: #endif // HAVE_STK

    factory.registerTypeT<NearestNoteNode>(concatenate(prefix, "music.nearest.note").c_str());
    factory.registerTypeT<MonodicNode>(concatenate(prefix, "music.monodic").c_str());

#ifdef HAVE_GLIB
    factory.registerTypeT<ShellNode>(concatenate(prefix, "os.shell").c_str());
#endif // HAVE_GLIB

// TODO #ifdef HAVE_LIBLO
    factory.registerTypeT<OscReceiverNode>(concatenate(prefix, "osc.receive").c_str());
    factory.registerTypeT<OscSenderNode>(concatenate(prefix, "osc.send").c_str());
// TODO #endif // HAVE_LIBLO
    factory.registerTypeT<OscPrependNode>(concatenate(prefix, "osc.prepend").c_str());
    factory.registerTypeT<OscRouteNode>(concatenate(prefix, "osc.route").c_str());

    factory.registerTypeT<SamplerSampleNode>(concatenate(prefix, "sampler.sample").c_str());
    factory.registerTypeT<SamplerNode>(concatenate(prefix, "sampler.simple").c_str());
    factory.registerTypeT<SamplerReadNode>(concatenate(prefix, "sampler.read").c_str());
    factory.registerTypeT<SamplerWriteNode>(concatenate(prefix, "sampler.write").c_str());

    factory.registerTypeT<StringCharactersNode>(concatenate(prefix,"string.characters").c_str());
    factory.registerTypeT<StringJoinNode>(concatenate(prefix,"string.join").c_str());
    factory.registerTypeT<StringSplitNode>(concatenate(prefix,"string.split").c_str());

#ifdef HAVE_GLIB
    factory.registerTypeT<RandomDrunkNode>(concatenate(prefix, "random.drunk").c_str());
    factory.registerTypeT<RandomIntNode>(concatenate(prefix, "random.int").c_str());
#endif // HAVE_GLIB

    factory.registerTypeT<AbstractionNode>(concatenate(prefix, "abs.abstraction").c_str());
    factory.registerTypeT<OutletNode>(concatenate(prefix, "abs.outlet").c_str());
    factory.registerTypeT<InletNode>(concatenate(prefix, "abs.inlet").c_str());

#ifdef HAVE_LIBMAPPER
    factory.registerTypeT<MapperInputNode>(concatenate(prefix, "osc.libmapper.input").c_str());
#endif // HAVE_LIBMAPPER

    factory.registerTypeT<SerialDeviceNode>(concatenate(prefix, "serial.device").c_str());
    factory.registerTypeT<ToDmxUsbProNode>(concatenate(prefix, "serial.todmxusbpro").c_str());
    factory.registerTypeT<BytesToBlobNode>(concatenate(prefix, "serial.bytestoblob").c_str());
    factory.registerTypeT<BlobToBytesNode>(concatenate(prefix, "serial.blobtobytes").c_str());
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


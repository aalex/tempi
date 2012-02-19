AM_CXXFLAGS = \
	$(LOG4CPP_CFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(LIBLO_CFLAGS) \
    $(LIBXML_CFLAGS) \
    $(SPATOSC_CFLAGS) \
    $(STK_CXXFLAGS) \
    -I$(top_srcdir) \
    -I$(top_srcdir)/tempi

AM_LIBS = \
	$(LOG4CPP_LIBS) \
    $(BOOST_FILESYSTEM_LIB) \
    $(BOOST_LDFLAGS) \
    $(BOOST_LIBS)  \
    $(BOOST_SYSTEM_LIB) \
    $(BOOST_THREAD_LIB) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(LIBLO_LIBS) \
    $(SPATOSC_LIBS) \
    $(LIBXML_LIBS) \
    $(STK_LIBS)

# this file is not installed, just included by .cpp files at compile time.
EXTRA_DIST = config.h

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libtempi-@TEMPI_API_VERSION@.la

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libtempi_@TEMPI_API_VERSION@_la_LDFLAGS = -export-dynamic -version-info $(TEMPI_LT_VERSION) $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_la_LIBS = $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_la_CXXFLAGS = $(AM_CXXFLAGS)
libtempi_@TEMPI_API_VERSION@_la_includedir = $(includedir)/tempi-$(TEMPI_API_VERSION)/tempi

## Define the source file list for the "libtempi-@TEMPI_API_VERSION@.la"
## target.  Note that @TEMPI_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_SOURCES = \
    attribute.cpp \
    base/anynode.cpp \
    base/appendnode.h \
    base/appsinknode.cpp \
    base/baselibrary.cpp \
    base/counternode.cpp \
    base/castnode.h \
    base/loadmessnode.cpp \
    base/delaynode.cpp \
    base/metro_node.cpp \
    base/nop_node.cpp \
    base/prependnode.cpp \
    base/print_node.cpp \
    base/routenode.cpp \
    base/spigotnode.cpp \
    base/spatosc/spatoscnode.h \
    clutter/clutterlibrary.cpp \
    clutter/clutterstagenode.cpp \
    documented.cpp \
    mapping/filter.cpp \
    fsm/rule.cpp \
    fsm/state.cpp \
    fsm/statemachine.cpp \
    graph.cpp \
    internals.cpp \
	log.cpp \
    mapping/slide.cpp \
    mapping/slidefilter.cpp \
    math/mathlibrary.cpp \
    message.cpp \
    midi/midiinput.cpp \
    midi/midilibrary.cpp \
    midi/midireceivernode.cpp \
    midi/midiroutenode.cpp \
    midi/midioutput.cpp \
    midi/midisendernode.cpp \
    midi/midiutilities.cpp \
    namedobject.cpp \
    node.cpp \
    nodefactory.cpp \
    nodesignal.cpp \
    entity.cpp \
    osc/osclibrary.cpp \
    osc/oscreceiver.cpp \
    osc/oscreceivernode.cpp \
    osc/oscsender.cpp \
    osc/oscsendernode.cpp \
    pad.cpp \
    sampler/samplerlibrary.cpp \
    sampler/loopingplayback.cpp \
    sampler/marker.cpp \
    sampler/pingpongplayback.cpp \
    sampler/player.cpp \
    sampler/region.cpp \
    sampler/recorder.cpp \
    sampler/samplernode.cpp \
    sampler/simpleplayback.cpp \
    sampler/score.cpp \
    sampler/track.cpp \
    scheduler.cpp \
    serializer.cpp \
    inlet.cpp \
    outlet.cpp \
    table/table.cpp \
    table/table_player.cpp \
    table/table_writer.cpp \
    tempi.cpp \
    timeposition.cpp \
    timer.cpp \
    threadedscheduler.cpp \
    utils.cpp

# TODO base/receivenode.cpp
# TODO base/receivenode.h

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    attribute.h \
    base/anynode.h \
    base/appsinknode.h \
    base/baselibrary.h \
    base/counternode.h \
    base/loadmessnode.h \
    base/delaynode.h \
    base/metro_node.h \
    base/nop_node.h \
    base/prependnode.h \
    base/print_node.h \
    base/routenode.h \
    base/spigotnode.h \
    clutter/clutterlibrary.h \
    clutter/clutterstagenode.h \
    concurrentqueue.h \
    documented.h \
    exceptions.h \
    mapping/filter.h \
    fsm/rule.h \
    fsm/state.h \
    fsm/statemachine.h \
    graph.h \
    identifier.h \
    internals.h \
	log.h \
    library.h \
    mapping/average.h \
    mapping/slide.h \
    mapping/slidefilter.h \
    math/mathlibrary.h \
    message.h \
    midi/midiinput.h \
    midi/midilibrary.h \
    midi/midireceivernode.h \
    midi/midiroutenode.h \
    midi/midioutput.h \
    midi/midisendernode.h \
    midi/midiutilities.h \
    namedobject.h \
    namedobjectmap.h \
    node.h \
    nodefactory.h \
    nodesignal.h \
    nodetype.h \
    osc/osclibrary.h \
    osc/oscreceiver.h \
    osc/oscreceivernode.h \
    osc/oscsender.h \
    osc/oscsendernode.h \
    sampler/samplerlibrary.h \
    sampler/loopingplayback.h \
    sampler/marker.h \
    sampler/pingpongplayback.h \
    sampler/playback.h \
    sampler/player.h \
    sampler/region.h \
    sampler/recorder.h \
    sampler/samplernode.h \
    sampler/simpleplayback.h \
    sampler/score.h \
    sampler/track.h \
    scheduler.h \
    serializer.h \
    sharedptr.h \
    inlet.h \
    entity.h \
    outlet.h \
    pad.h \
    table/table.h \
    table/table_player.h \
    table/table_writer.h \
    tempi-version.h \
    tempi.h \
    timeposition.h \
    timer.h \
    threadedscheduler.h \
    utils.h


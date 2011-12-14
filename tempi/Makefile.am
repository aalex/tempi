AM_CXXFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(LIBLO_CFLAGS) \
    $(LIBXML_CFLAGS) \
    $(STK_CXXFLAGS) \
    -I$(top_srcdir) \
    -I$(top_srcdir)/tempi

AM_LIBS = \
    $(BOOST_LDFLAGS) \
    $(BOOST_THREAD_LIB) \
    $(LIBLO_LIBS) \
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
    base/anynode.cpp \
    base/appsinknode.cpp \
    base/baselibrary.cpp \
    base/counternode.cpp \
    base/metro_node.cpp \
    base/nop_node.cpp \
    base/prependnode.cpp \
    base/print_node.cpp \
    base/receivenode.h \
    mapping/filter.cpp \
    fsm/rule.cpp \
    fsm/state.cpp \
    fsm/statemachine.cpp \
    graph.cpp \
    internals.cpp \
    mapping/slide.cpp \
    mapping/slidefilter.cpp \
    message.cpp \
    midi/midiinput.cpp \
    midi/midilibrary.cpp \
    midi/midireceivernode.cpp \
    midi/midioutput.cpp \
    midi/midisendernode.cpp \
    node.cpp \
    nodefactory.cpp \
    osc/osclibrary.cpp \
    osc/oscreceiver.cpp \
    osc/oscreceivernode.cpp \
    osc/oscsender.cpp \
    osc/oscsendernode.cpp \
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
    sink.cpp \
    source.cpp \
    table/table.cpp \
    table/table_player.cpp \
    table/table_writer.cpp \
    tempi.cpp \
    timeposition.cpp \
    timer.cpp \
    threadedscheduler.cpp \
	utils.cpp

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    base/anynode.h \
    base/appsinknode.h \
    base/baselibrary.h \
    base/counternode.h \
    base/metro_node.h \
    base/nop_node.h \
    base/prependnode.h \
    base/print_node.h \
    base/receivenode.h \
    concurrentqueue.h \
    exceptions.h \
    mapping/filter.h \
    fsm/rule.h \
    fsm/state.h \
    fsm/statemachine.h \
    graph.h \
    identifier.h \
    internals.h \
    library.h \
    mapping/average.h \
    mapping/slide.h \
    mapping/slidefilter.h \
    message.h \
    midi/midiinput.h \
    midi/midilibrary.h \
    midi/midireceivernode.h \
    midi/midioutput.h \
    midi/midisendernode.h \
    node.h \
    nodefactory.h \
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
    sharedptr.h \
    sink.h \
    source.h \
    table/table.h \
    table/table_player.h \
    table/table_writer.h \
    tempi-version.h \
    tempi.h \
    timeposition.h \
    timer.h \
    threadedscheduler.h \
    utils.h


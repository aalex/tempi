SUBDIRS = tests

AM_CXXFLAGS = \
    $(LOG4CPP_CFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(ATOM_CFLAGS) \
    $(LIBMAPPER_CFLAGS) \
    $(LIBLO_CFLAGS) \
    $(LIBXML_CFLAGS) \
    $(SPATOSC_CFLAGS) \
    $(STK_CXXFLAGS) \
    $(V8_CFLAGS) \
    -I$(top_srcdir)

AM_LIBS = \
    $(BOOST_FILESYSTEM_LIBS) \
    $(BOOST_LIBS)  \
    $(BOOST_REGEX_LIBS)  \
    $(BOOST_SYSTEM_LIBS) \
    $(ATOM_LIBS) \
    $(BOOST_THREAD_LIBS) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(LIBMAPPER_LIBS) \
    $(LIBLO_LIBS) \
    $(LIBXML_LIBS) \
    $(LOG4CPP_LIBS) \
    $(SPATOSC_LIBS) \
    $(STK_LIBS) \
    $(V8_LIBS)

LDADD = $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libtempi-@TEMPI_API_VERSION@-plugins-base.la

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libtempi_@TEMPI_API_VERSION@_plugins_base_la_LDFLAGS = -export-dynamic -version-info $(TEMPI_LT_VERSION) $(AM_LIBS) $(LDADD)
libtempi_@TEMPI_API_VERSION@_plugins_base_la_LIBS = $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_plugins_base_la_CXXFLAGS = $(AM_CXXFLAGS)
libtempi_@TEMPI_API_VERSION@_plugins_base_la_includedir = $(includedir)/tempi-$(TEMPI_API_VERSION)

## Define the source file list for the "libtempi-@TEMPI_API_VERSION@.la"
## target.  Note that @TEMPI_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_plugins_base_la_SOURCES = \
    3d/3dcomparenode.cpp \
	abstraction/abstractionnode.cpp \
	abstraction/inletnode.cpp \
	abstraction/outletnode.cpp \
    baselibrary.cpp \
    clutter/clutterstagenode.cpp \
	edit/grapheditornode.cpp \
    flow/anynode.cpp \
    flow/appsinknode.cpp \
    flow/counternode.cpp \
    flow/delaynode.cpp \
    flow/linenode.cpp \
    flow/loadmessnode.cpp \
    flow/metro_node.cpp \
    flow/nop_node.cpp \
    flow/packnode.cpp \
    flow/prependnode.cpp \
    flow/print_node.cpp \
    flow/routenode.cpp \
    flow/spigotnode.cpp \
    flow/splitnode.cpp \
    flow/typenode.cpp \
    flow/triggernode.cpp \
    flow/unpacknode.cpp \
	mapper/mapperinputnode.cpp \
    mapping/filter.cpp \
    mapping/slide.cpp \
    mapping/slidefilter.cpp \
    math/booleanoperatornode.cpp \
    math/expr.cpp \
    math/onefloatmathnode.cpp \
    math/twofloatmathnode.cpp \
    midi/abstractmidifilter.cpp \
    midi/midibuildernodes.cpp \
    midi/mididevicelistermethod.cpp \
    midi/midireceivernode.cpp \
    midi/midiroutenode.cpp \
    midi/midisendernode.cpp \
    midi/midiutilities.cpp \
    music/monodicnode.cpp \
    os/shellnode.cpp \
    osc/oscprependnode.cpp \
    osc/oscreceivernode.cpp \
    osc/oscroutenode.cpp \
    osc/oscsendernode.cpp \
    random/drunkint.cpp \
    sampler/samplenode.cpp \
    sampler/samplernode.cpp \
    sampler/samplerreadnode.cpp \
    sampler/samplerwritenode.cpp \
    serial/serialdevice.cpp \
    serial/serialdevicenode.cpp \
    string/stringcharactersnode.cpp \
    string/stringjoinnode.cpp \
    string/stringsplitnode.cpp

EXTRA_DIST = \
    3d/3dcomparenode.h \
	abstraction/abstractionnode.h \
	abstraction/inletnode.h \
	abstraction/outletnode.h \
    baselibrary.h \
    clutter/clutterstagenode.h \
	edit/grapheditornode.h \
    flow/anynode.h \
    flow/appendnode.h \
    flow/appsinknode.h \
    flow/castnode.h \
    flow/counternode.h \
    flow/delaynode.h \
    flow/dictnode.h \
    flow/linenode.h \
    flow/loadmessnode.h \
    flow/metro_node.h \
    flow/nop_node.h \
    flow/packnode.h \
    flow/prependnode.h \
    flow/print_node.h \
    flow/routenode.h \
    flow/spigotnode.h \
    flow/splitnode.h \
    flow/typenode.h \
    flow/triggernode.h \
    flow/unpacknode.h \
	mapper/mapperdevice.h \
	mapper/mapperinputnode.h \
    mapping/average.h \
    mapping/filter.h \
    mapping/slide.h \
    mapping/slidefilter.h \
    math/booleanoperatornode.h \
    math/expr.h \
    math/onefloatmathnode.h \
    math/twofloatmathnode.h \
    music/musicnearestnode.h \
    music/monodicnode.h \
    os/shellnode.h \
    midi/midireceivernode.h \
    midi/abstractmidifilter.h \
    midi/midibuildernodes.h \
    midi/mididevicelistermethod.h \
    midi/midiroutenode.h \
    midi/midisendernode.h \
    midi/midiutilities.h \
    osc/oscprependnode.h \
    osc/oscreceivernode.h \
    osc/oscroutenode.h \
    osc/oscsendernode.h \
    random/drunkint.h \
    random/randomintnode.h \
    random/randomdrunknode.h \
    sampler/samplenode.h \
    sampler/samplernode.h \
    sampler/samplerreadnode.h \
    sampler/samplerwritenode.h \
    spatosc/spatoscnode.h \
    string/stringcharactersnode.h \
    string/stringjoinnode.h \
    string/stringsplitnode.h

# TODO base/receivenode.cpp
# TODO base/receivenode.h


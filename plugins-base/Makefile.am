AM_CXXFLAGS = \
    $(LOG4CPP_CFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(LIBLO_CFLAGS) \
    $(LIBXML_CFLAGS) \
    $(SPATOSC_CFLAGS) \
    $(STK_CXXFLAGS) \
    -I$(top_srcdir) \
    -I$(top_srcdir)/plugins-base \
    -I$(top_srcdir)/tempi

AM_LIBS = \
    $(BOOST_FILESYSTEM_LIB) \
    $(BOOST_LDFLAGS) \
    $(BOOST_LIBS)  \
    $(BOOST_SYSTEM_LIB) \
    $(BOOST_THREAD_LIB) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(LIBLO_LIBS) \
    $(LIBXML_LIBS) \
    $(LOG4CPP_LIBS) \
    $(SPATOSC_LIBS) \
    $(STK_LIBS)

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libtempi-plugins-base.la

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libtempi_plugins_base_la_LDFLAGS = -export-dynamic -version-info $(TEMPI_LT_VERSION) $(AM_LIBS)
libtempi_plugins_base_la_LIBS = $(AM_LIBS)
libtempi_plugins_base_la_CXXFLAGS = $(AM_CXXFLAGS)
libtempi_plugins_base_la_includedir = $(includedir)/tempi-$(TEMPI_API_VERSION)

## Define the source file list for the "libtempi-@TEMPI_API_VERSION@.la"
## target.  Note that @TEMPI_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_plugins_base_la_SOURCES = \
    baselibrary.cpp \
    clutter/clutterstagenode.cpp \
    flow/anynode.cpp \
    flow/appsinknode.cpp \
    flow/counternode.cpp \
    flow/delaynode.cpp \
    flow/loadmessnode.cpp \
    flow/metro_node.cpp \
    flow/nop_node.cpp \
    flow/prependnode.cpp \
    flow/print_node.cpp \
    flow/routenode.cpp \
    flow/spigotnode.cpp \
    mapping/filter.cpp \
    mapping/slide.cpp \
    mapping/slidefilter.cpp \
    midi/midireceivernode.cpp \
    midi/midiroutenode.cpp \
    midi/midisendernode.cpp \
    midi/midiutilities.cpp \
    osc/oscreceivernode.cpp \
    osc/oscsendernode.cpp \
    random/drunkint.cpp \
    sampler/samplernode.cpp

EXTRA_DIST = \
    baselibrary.h \
    clutter/clutterstagenode.h \
    flow/anynode.h \
    flow/appendnode.h \
    flow/appsinknode.h \
    flow/castnode.h \
    flow/counternode.h \
    flow/delaynode.h \
    flow/dictnode.h \
    flow/loadmessnode.h \
    flow/metro_node.h \
    flow/nop_node.h \
    flow/prependnode.h \
    flow/print_node.h \
    flow/routenode.h \
    flow/spigotnode.h \
    mapping/average.h \
    mapping/filter.h \
    mapping/slide.h \
    mapping/slidefilter.h \
    math/mathnodes.h \
    midi/midireceivernode.h \
    midi/midiroutenode.h \
    midi/midisendernode.h \
    midi/midiutilities.h \
    osc/oscreceivernode.h \
    osc/oscsendernode.h \
    random/drunkint.h \
    random/randomdrunknode.h \
    sampler/samplernode.h \
    spatosc/spatoscnode.h

# TODO base/receivenode.cpp
# TODO base/receivenode.h

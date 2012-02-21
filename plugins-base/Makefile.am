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
    baselibrary.h \
    clutter/clutterstagenode.cpp \
    clutter/clutterstagenode.h \
    flow/anynode.cpp \
    flow/anynode.h \
    flow/appendnode.h \
    flow/appsinknode.cpp \
    flow/appsinknode.h \
    flow/castnode.h \
    flow/counternode.cpp \
    flow/counternode.h \
    flow/delaynode.cpp \
    flow/delaynode.h \
    flow/loadmessnode.cpp \
    flow/loadmessnode.h \
    flow/metro_node.cpp \
    flow/metro_node.h \
    flow/nop_node.cpp \
    flow/nop_node.h \
    flow/prependnode.cpp \
    flow/prependnode.h \
    flow/print_node.cpp \
    flow/print_node.h \
    flow/routenode.cpp \
    flow/routenode.h \
    flow/spigotnode.cpp \
    flow/spigotnode.h \
    mapping/average.h \
    mapping/filter.cpp \
    mapping/filter.h \
    mapping/slide.cpp \
    mapping/slide.h \
    mapping/slidefilter.cpp \
    mapping/slidefilter.h \
    math/mathnodes.h \
    midi/midireceivernode.cpp \
    midi/midireceivernode.h \
    midi/midiroutenode.cpp \
    midi/midiroutenode.h \
    midi/midisendernode.cpp \
    midi/midisendernode.h \
    midi/midiutilities.cpp \
    midi/midiutilities.h \
    osc/oscreceivernode.cpp \
    osc/oscreceivernode.h \
    osc/oscsendernode.cpp \
    osc/oscsendernode.h \
    sampler/samplernode.cpp \
    spatosc/spatoscnode.h \
    sampler/samplernode.h

# TODO base/receivenode.cpp
# TODO base/receivenode.h


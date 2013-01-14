SUBDIRS = tests

AM_CXXFLAGS = \
    $(LOG4CPP_CFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(LIBXML_CFLAGS) \
    $(LIBLO_CFLAGS) \
    $(ATOM_CFLAGS) \
    -I$(top_srcdir) \
    -I$(top_srcdir)/plugins-base \
    -I$(top_srcdir)/tempi \
    $(PORTMIDI_CFLAGS) \
    $(RTMIDI_CFLAGS)

# XXX BOOST_FILESYSTEM_LIBS must be after BOOST_SYSTEM_LIBS

AM_LIBS = \
    $(BOOST_LIBS)  \
    $(BOOST_REGEX_LIBS)  \
    $(BOOST_SYSTEM_LIBS) \
    $(BOOST_FILESYSTEM_LIBS) \
    $(BOOST_THREAD_LIBS) \
    $(LIBLO_LIBS) \
    $(LIBXML_LIBS) \
    $(ATOM_LIBS) \
    $(LOG4CPP_LIBS) \
    $(RTMIDI_LIBS) \
    $(PORTMIDI_LIBS)

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
    documented.cpp \
    dict.cpp \
	dynamicpatching.cpp \
    entity.cpp \
    entitymethod.cpp \
    entitysignal.cpp \
    graph.cpp \
    inlet.cpp \
    internals.cpp \
    loader.cpp \
    log.cpp \
    message.cpp \
    midi.cpp \
    midi_scheduler.cpp \
    nameallocator.cpp \
    namedobject.cpp \
    namevalidator.cpp \
    node.cpp \
    nodefactory.cpp \
    oscreceiver.cpp \
    oscsender.cpp \
    outlet.cpp \
    pad.cpp \
    sampler_loopingplayback.cpp \
    sampler_marker.cpp \
    sampler_pingpongplayback.cpp \
    sampler_player.cpp \
    sampler_recorder.cpp \
    sampler_region.cpp \
    sampler_score.cpp \
    sampler_simpleplayback.cpp \
    sampler_track.cpp \
    scheduler.cpp \
    serializer.cpp \
    synchronousscheduler.cpp \
    table.cpp \
    table_player.cpp \
    table_writer.cpp \
    tempi.cpp \
    threadedscheduler.cpp \
    timeposition.cpp \
    timer.cpp \
    utils.cpp \
    wrapper.cpp

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    attribute.h \
    concurrentqueue.h \
    dict.h \
	dynamicpatching.h \
    documented.h \
    entity.h \
    entitymethod.h \
    entitysignal.h \
    exceptions.h \
    graph.h \
    oscreceiver.h \
    oscsender.h \
    identifier.h \
    inlet.h \
    internals.h \
    library.h \
    loader.h \
    log.h \
    message.h \
    midi.h \
    midi_scheduler.h \
    nameallocator.h \
    namedobject.h \
    namedobjectmap.h \
    namevalidator.h \
    node.h \
    nodefactory.h \
    nodetype.h \
    outlet.h \
    pad.h \
    sampler_loopingplayback.h \
    sampler_marker.h \
    sampler_pingpongplayback.h \
    sampler_playback.h \
    sampler_player.h \
    sampler_recorder.h \
    sampler_region.h \
    sampler_score.h \
    sampler_simpleplayback.h \
    sampler_track.h \
    scheduler.h \
    serializer.h \
    synchronousscheduler.h \
    sharedptr.h \
    table.h \
    table_player.h \
    table_writer.h \
    tempi-version.h \
    tempi.h \
    threadedscheduler.h \
    timeposition.h \
    timer.h \
    utils.h \
    wrapper.h


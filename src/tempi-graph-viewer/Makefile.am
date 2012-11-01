bin_PROGRAMS = \
    tempi-graph-viewer
    

AM_CXXFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    -I$(top_srcdir)

LDADD = \
    $(BOOST_PROGRAM_OPTIONS_LIBS) \
    $(BOOST_SYSTEM_LIBS) \
    $(BOOST_REGEX_LIBS) \
    $(BOOST_FILESYSTEM_LIBS) \
    $(BOOST_THREAD_LIBS) \
    $(BOOST_LIBS) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_graph_viewer_CXXFLAGS = $(AM_CXXFLAGS)
tempi_graph_viewer_CFLAGS = \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS)

tempi_graph_viewer_LDADD = $(LDADD)

EXTRA_DIST = \
    miller-command.h \
    miller-draw.h \
    miller-macros.h \
	miller-connection.h

tempi_graph_viewer_SOURCES = \
    miller-draw.cpp \
	miller-connection.c \
    main.cpp


bin_PROGRAMS = \
    tempi-inspect

AM_CXXFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    -I$(top_srcdir)

LDADD = \
    $(BOOST_PROGRAM_OPTIONS_LIB) \
    $(BOOST_FILESYSTEM_LIB) \
    $(BOOST_SYSTEM_LIB) \
    $(BOOST_LDFLAGS) \
    $(BOOST_THREAD_LIB) \
    $(BOOST_LIBS) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(top_builddir)/plugins-base/libtempi-@TEMPI_API_VERSION@-plugins-base.la \
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_CXXFLAGS = $(AM_CXXFLAGS)
tempi_inspect_LDADD = $(LDADD)

tempi_inspect_SOURCES = \
    main.cpp


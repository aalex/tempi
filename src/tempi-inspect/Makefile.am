bin_PROGRAMS = \
    tempi-inspect

AM_CXXFLAGS = \
    $(ATOM_CFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    -I$(top_srcdir)

LDADD = \
    $(ATOM_LIBS) \
    $(BOOST_PROGRAM_OPTIONS_LIBS) \
    $(BOOST_SYSTEM_LIBS) \
    $(BOOST_REGEX_LIBS) \
    $(LUA_LIBS) \
    $(BOOST_FILESYSTEM_LIBS) \
    $(BOOST_THREAD_LIBS) \
    $(BOOST_LIBS) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_CXXFLAGS = $(AM_CXXFLAGS)
tempi_inspect_LDADD = $(LDADD)

tempi_inspect_SOURCES = \
    main.cpp


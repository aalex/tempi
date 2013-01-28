bin_PROGRAMS = \
    tempi-osc-forward

AM_CXXFLAGS = \
    $(ATOM_CFLAGS) \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    $(LIBLO_CFLAGS) \
    -I$(top_srcdir)

LDADD = \
    $(ATOM_LIBS) \
    $(BOOST_PROGRAM_OPTIONS_LIBS) \
    $(BOOST_LIBS) \
    $(CLUTTER_LIBS) \
    $(LUA_LIBS) \
    $(BOOST_REGEX_LIBS) \
    $(GLIB_LIBS) \
    $(LIBLO_LIBS) \
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_osc_forward_CXXFLAGS = $(AM_CXXFLAGS)
tempi_osc_forward_LDADD = $(LDADD)

tempi_osc_forward_SOURCES = \
    main.cpp


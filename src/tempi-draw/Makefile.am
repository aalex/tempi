bin_PROGRAMS = \
    tempi-draw

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
    $(BOOST_REGEX_LIBS) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(LIBLO_LIBS) \
    $(LUA_LIBS) \
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_draw_CXXFLAGS = $(AM_CXXFLAGS)
tempi_draw_LDADD = $(LDADD)

headers = \
    legacy.h \
    sampler.h \
    particlegenerator.h

tempi_draw_SOURCES = \
    main.cpp \
    sampler.cpp \
    particlegenerator.cpp \
    $(headers)


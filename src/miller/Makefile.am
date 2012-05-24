bin_PROGRAMS = \
    miller

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
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

miller_CXXFLAGS = $(AM_CXXFLAGS)
miller_LDADD = $(LDADD)

miller_SOURCES = \
    main.cpp


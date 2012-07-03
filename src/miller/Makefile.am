noinst_PROGRAMS = \
    miller

AM_CXXFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    -I$(top_srcdir)

LDADD = \
    $(BOOST_PROGRAM_OPTIONS_LIBS) \
    $(BOOST_SYSTEM_LIBS) \
    $(BOOST_FILESYSTEM_LIBS) \
    $(BOOST_THREAD_LIBS) \
    $(BOOST_LIBS) \
    $(CLUTTER_LIBS) \
    $(GLIB_LIBS) \
    $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

miller_CXXFLAGS = $(AM_CXXFLAGS)
miller_LDADD = $(LDADD)

EXTRA_DIST = \
    miller-macros.h \
    miller-command.h

miller_SOURCES = \
    main.cpp


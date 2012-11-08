bin_PROGRAMS = \
    tempi-midi-looper

AM_CXXFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(GLIB_CFLAGS) \
    -DDATADIR="\"$(datadir)\"" \
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

tempi_midi_looper_CXXFLAGS = $(AM_CXXFLAGS)
tempi_midi_looper_LDADD = $(LDADD)

tempi_midi_looper_SOURCES = \
    main.cpp

imagesdir = $(datadir)/tempi
images_DATA = background.png
EXTRA_DIST = $(images_DATA)


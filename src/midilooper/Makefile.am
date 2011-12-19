bin_PROGRAMS = \
	midilooper

AM_CXXFLAGS = \
	$(BOOST_CPPFLAGS) \
	$(CLUTTER_CFLAGS) \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)

LDADD = \
	$(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_LIBS) \
	$(CLUTTER_LIBS) \
	$(GLIB_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

midilooper_CXXFLAGS = $(AM_CXXFLAGS)
midilooper_LDADD = $(LDADD)

midilooper_SOURCES = \
	main.cpp

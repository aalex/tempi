bin_PROGRAMS = \
	midi-print

AM_CXXFLAGS = \
	$(BOOST_CPPFLAGS) \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)

LDADD = \
	$(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_LIBS) \
	$(CLUTTER_LIBS) \
	$(GLIB_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

midi_print_CXXFLAGS = $(AM_CXXFLAGS)
midi_print_LDADD = $(LDADD)

midi_print_SOURCES = \
	main.cpp


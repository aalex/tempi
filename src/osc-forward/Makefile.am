bin_PROGRAMS = \
	osc-forward

AM_CXXFLAGS = \
	$(BOOST_CPPFLAGS) \
	$(CLUTTER_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBLO_CFLAGS) \
	-I$(top_srcdir)

LDADD = \
	$(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_LIBS) \
	$(CLUTTER_LIBS) \
	$(GLIB_LIBS) \
	$(LIBLO_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

osc_forward_CXXFLAGS = $(AM_CXXFLAGS)
osc_forward_LDADD = $(LDADD)

osc_forward_SOURCES = \
	main.cpp


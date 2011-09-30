bin_PROGRAMS = tempi

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

tempi_CXXFLAGS = $(AM_CXXFLAGS)
tempi_LDADD = $(LDADD)

headers = \
    particlegenerator.h

tempi_SOURCES = \
	main.cpp \
    particlegenerator.cpp \
	$(headers)


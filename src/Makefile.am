bin_PROGRAMS = tempi

AM_CXXFLAGS = \
	$(CLUTTER_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBLO_CFLAGS) \
	$(LIBXML_CFLAGS) \
	-I$(top_srcdir)

LDADD = \
	$(CLUTTER_LIBS) \
	$(GLIB_LIBS) \
	$(LIBLO_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_CXXFLAGS = $(AM_CXXFLAGS)
tempi_LDADD = $(LDADD)

headers = \
    particlegenerator.h

tempi_SOURCES = \
	main.cpp \
    particlegenerator.cpp \
	$(headers)


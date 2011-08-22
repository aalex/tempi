bin_PROGRAMS = tempi

AM_CXXFLAGS = \
	$(CLUTTER_CFLAGS) \
	$(LIBXML_CFLAGS) \
	-I$(top_srcdir)/tempi

LDADD = \
	$(CLUTTER_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_CXXFLAGS = $(AM_CXXFLAGS)
tempi_LDADD = $(LDADD)

headers = \
	gui.h

tempi_SOURCES = \
	gui.cpp \
	main.cpp \
	$(headers)



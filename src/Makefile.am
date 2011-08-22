bin_PROGRAMS = tempi

AM_CFLAGS = \
	$(CLUTTER_CFLAGS) \
	$(LIBXML_CFLAGS) \
	-I$(top_srcdir)/tempi

LDADD = \
	$(CLUTTER_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

headers = \
	gui.h

tempi_SOURCES = \
	gui.cpp \
	main.cpp \
	$(headers)



bin_PROGRAMS = tempi

AM_CXXFLAGS = \
	$(CLUTTER_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(LIBXML_CFLAGS) \
	-I$(top_srcdir)

LDADD = \
	$(CLUTTER_LIBS) \
	$(GLIB_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

tempi_CXXFLAGS = $(AM_CXXFLAGS)
tempi_LDADD = $(LDADD)

headers = \
    sampler.h \
    recorder.h \
	gui.h

tempi_SOURCES = \
	gui.cpp \
    sampler.cpp \
    recorder.cpp \
	main.cpp \
	$(headers)



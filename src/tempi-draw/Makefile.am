bin_PROGRAMS = \
	tempi-draw

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

tempi_draw_CXXFLAGS = $(AM_CXXFLAGS)
tempi_draw_LDADD = $(LDADD)

headers = \
    legacy.h \
	sampler.h \
    particlegenerator.h

tempi_draw_SOURCES = \
	main.cpp \
	sampler.cpp \
    particlegenerator.cpp \
	$(headers)


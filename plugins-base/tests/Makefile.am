AM_CXXFLAGS = \
    $(ATOM_CFLAGS) \
	-I$(top_srcdir)

LDADD = \
    $(top_builddir)/plugins-base/libtempi-@TEMPI_API_VERSION@-plugins-base.la \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

check_PROGRAMS = \
    check_average \
    check_base \
    check_line \
    check_math \
    check_slide

TESTS = $(check_PROGRAMS)

check_average_SOURCES = check_average.cpp
check_base_SOURCES = check_base.cpp
check_line_SOURCES = check_line.cpp
check_math_SOURCES = check_math.cpp
check_slide_SOURCES = check_slide.cpp


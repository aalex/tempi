AM_CXXFLAGS = -I$(top_srcdir)
LDADD = \
    $(top_builddir)/plugins-base/libtempi-plugins-base.la \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

check_PROGRAMS = \
    check_base \
    check_math

TESTS = $(check_PROGRAMS)

check_base_SOURCES = check_base.cpp
check_math_SOURCES = check_math.cpp


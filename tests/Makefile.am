AM_CXXFLAGS = -I$(top_srcdir)
LDADD = $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

check_PROGRAMS = \
    check_fsm \
    check_timer \
    check_track \
    check_version

TESTS = $(check_PROGRAMS)

check_version_SOURCES = check_version.cpp
check_fsm_SOURCES = check_fsm.cpp
check_timer_SOURCES = check_timer.cpp
check_track_SOURCES = check_track.cpp

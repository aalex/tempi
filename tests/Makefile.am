AM_CXXFLAGS = -I$(top_srcdir)
LDADD = $(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

check_PROGRAMS = \
    check_average \
    check_fsm \
    check_message \
    check_node \
    check_properties \
    check_slide \
    check_timer \
    check_track \
    check_version

TESTS = $(check_PROGRAMS)

check_average_SOURCES = check_average.cpp
check_fsm_SOURCES = check_fsm.cpp
check_message_SOURCES = check_message.cpp
check_node_SOURCES = check_node.cpp
check_properties_SOURCES = check_properties.cpp
check_slide_SOURCES = check_slide.cpp
check_timer_SOURCES = check_timer.cpp
check_track_SOURCES = check_track.cpp
check_version_SOURCES = check_version.cpp

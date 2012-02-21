AM_CXXFLAGS = -I$(top_srcdir)
LDADD = \
    $(top_builddir)/plugins-base/libtempi-plugins-base.la \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

check_PROGRAMS = \
    check_average \
    check_base \
    check_fsm \
    check_graph \
    check_math \
    check_message \
    check_libraries \
    check_log \
    check_nodefactory \
    check_osc \
    check_properties \
    check_serializer \
    check_slide \
    check_table \
    check_timer \
    check_track \
    check_version

TESTS = $(check_PROGRAMS)

check_average_SOURCES = check_average.cpp
check_base_SOURCES = check_base.cpp
check_fsm_SOURCES = check_fsm.cpp
check_graph_SOURCES = check_graph.cpp
check_math_SOURCES = check_math.cpp
check_message_SOURCES = check_message.cpp
check_libraries_SOURCES = check_libraries.cpp
check_log_SOURCES = check_log.cpp
check_node_SOURCES = check_node.cpp
check_nodefactory_SOURCES = check_nodefactory.cpp
check_osc_SOURCES = check_osc.cpp
check_properties_SOURCES = check_properties.cpp
check_serializer_SOURCES = check_serializer.cpp
check_slide_SOURCES = check_slide.cpp
check_table_SOURCES = check_table.cpp
check_timer_SOURCES = check_timer.cpp
check_track_SOURCES = check_track.cpp
check_version_SOURCES = check_version.cpp


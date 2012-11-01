AM_CXXFLAGS = -I$(top_srcdir)
LDADD = \
    $(top_builddir)/plugins-base/libtempi-@TEMPI_API_VERSION@-plugins-base.la \
	$(top_builddir)/tempi/libtempi-@TEMPI_API_VERSION@.la

check_PROGRAMS = \
    check_graph \
    check_message \
    check_libraries \
    check_log \
    check_namevalidator \
    check_nodefactory \
    check_osc \
    check_properties \
    check_serializer \
    check_table \
    check_timer \
    check_track \
    check_version \
    check_wrapper

TESTS = $(check_PROGRAMS)

check_graph_SOURCES = check_graph.cpp
check_message_SOURCES = check_message.cpp
check_libraries_SOURCES = check_libraries.cpp
check_log_SOURCES = check_log.cpp
check_namevalidator_SOURCES = check_namevalidator.cpp
check_nodefactory_SOURCES = check_nodefactory.cpp
check_osc_SOURCES = check_osc.cpp
check_properties_SOURCES = check_properties.cpp
check_serializer_SOURCES = check_serializer.cpp
check_table_SOURCES = check_table.cpp
check_timer_SOURCES = check_timer.cpp
check_track_SOURCES = check_track.cpp
check_version_SOURCES = check_version.cpp
check_wrapper_SOURCES = check_wrapper.cpp


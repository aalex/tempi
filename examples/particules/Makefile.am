bin_PROGRAMS = partickle

noinst_HEADERS = \
    base_application.h \
    particles_application.h \
    text_renderer.h

partickle_SOURCES = \
    base_application.cpp \
    particles_application.cpp \
    text_renderer.cpp

partickle_CPPFLAGS = -I$(top_srcdir)

partickle_CXXFLAGS = \
    $(OGRE_CFLAGS) \
    $(OIS_CFLAGS) \
    $(TEMPI_CFLAGS) \
    -Wall -Werror -Wfatal-errors

partickle_LDADD = \
    $(OGRE_LIBS) \
    $(OIS_LIBS) \
    $(TEMPI_LIBS)

EXTRA_DIST = autogen.sh


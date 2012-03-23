AM_CPPFLAGS = \
    $(BOOST_CPPFLAGS) \
    -I$(top_srcdir)

AM_LIBS = \
    $(BOOST_PYTHON_LIB) \
    $(BOOST_LDFLAGS) \
    $(BOOST_LIBS)  \
    $(BOOST_SYSTEM_LIB) \
    $(BOOST_THREAD_LIB)

python_LTLIBRARIES = libtempy.la
libtempy_la_SOURCES = python-tempi.cpp
libtempy_la_LIBADD = $(top_builddir)/tempi/libtempi_@TEMPI_API_VERSION@.la
libtempydir = `python -c 'from distutils import sysconfig; print(sysconfig.get_python_lib(1, 0);'`


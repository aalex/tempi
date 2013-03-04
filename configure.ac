# Process this file with autoconf to produce a configure script.

# tempi package version number, (as distinct from shared library version)
# An odd micro number indicates in-progress development from Git
# An even micro number indicates a released version
#
# Making a point release:
# - increase tempi_micro_version to the next even number
# - increase tempi_interface_age to the next even number
#   UNLESS there was an API addition/deprecation, in which case
#   - set tempi_interface_age to 0
# After the release:
# - increase tempi_micro_version to the next odd number
# - increase tempi_interface_version to the next odd number
m4_define([tempi_major_version], [0])
m4_define([tempi_minor_version], [3])
m4_define([tempi_micro_version], [1])

m4_define([tempi_release_status],
          [m4_if(m4_eval(tempi_micro_version % 2), [1], [git],
                 [m4_if(m4_eval(tempi_minor_version % 2), [1], [snapshot],
                                                                 [release])])])

m4_define([tempi_version], [tempi_major_version.tempi_minor_version.tempi_micro_version])

# change this only when breaking the API
m4_define([tempi_api_version], [0.3])

# increase the interface age by 1 for each release; if the API changes,
# set to 0. interface_age and binary_age are used to create the soname
# of the shared object:
#
#  (<minor> * 100 + <micro>) - <interface_age>
#
# this allows using the same soname for different micro-releases in case
# no API was added or deprecated. for instance:
#
#   tempi 1.2.0  -> 100 * 2 + 0  = 200, interface age = 0 -> 200
#   tempi 1.2.2  -> 100 * 2 + 2  = 202, interface age = 2 -> 200
#   tempi 1.2.4  -> 100 * 2 + 4  = 204, interface age = 4 -> 200
#   [ API addition, deprecation ]
#   tempi 1.2.6  -> 100 * 2 + 6  = 206, interface age = 0 -> 206
#   tempi 1.2.8  -> 100 * 2 + 8  = 208, interface age = 2 -> 206
#   tempi 1.2.10 -> 100 * 2 + 10 = 210, interface age = 4 -> 206
#   ...
#
m4_define([tempi_interface_age], [0])
m4_define([tempi_binary_age], [m4_eval(100 * tempi_minor_version + tempi_micro_version)])

AC_PREREQ([2.63])
AC_INIT([tempi],
        [tempi_version],
        [http://code.sat.qc.cq/redmine/projects/tempi/issues],
        [tempi],
        [http://code.sat.qc.ca/tempi])
AC_RUN_IFELSE([AC_LANG_PROGRAM([[ ]], [[ ]])],
    [AC_MSG_RESULT(yes)], [AC_MSG_RESULT(no)], [AC_MSG_RESULT(can not test)])
AC_CONFIG_MACRO_DIR([m4])
AC_MSG_NOTICE([Configuring tempi])
AM_INIT_AUTOMAKE([subdir-objects -Wno-portability])
AC_CONFIG_HEADERS([tempi/config.h])
AC_CANONICAL_HOST # fills $host variable

AC_SUBST([TEMPI_MAJOR_VERSION],  [tempi_major_version])
AC_SUBST([TEMPI_MINOR_VERSION],  [tempi_minor_version])
AC_SUBST([TEMPI_MICRO_VERSION],  [tempi_micro_version])
AC_SUBST([TEMPI_VERSION],        [tempi_version])
AC_SUBST([TEMPI_API_VERSION],    [tempi_api_version])
AC_SUBST([TEMPI_API_VERSION_AM], [$TEMPI_MAJOR_VERSION\_0])
AC_SUBST([TEMPI_RELEASE_STATUS], [tempi_release_status])

m4_define([lt_current],  [m4_eval(100 * tempi_minor_version + tempi_micro_version - tempi_interface_age)])
m4_define([lt_revision], [tempi_interface_age])
m4_define([lt_age],      [m4_eval(tempi_binary_age - tempi_interface_age)])
TEMPI_LT_CURRENT=lt_current
TEMPI_LT_REV=lt_revision
TEMPI_LT_AGE=lt_age
TEMPI_LT_VERSION="$TEMPI_LT_CURRENT:$TEMPI_LT_REV:$TEMPI_LT_AGE"
AC_SUBST([TEMPI_LT_VERSION], "$TEMPI_LT_VERSION")
TEMPI_LT_LDFLAGS="-version-info $TEMPI_LT_VERSION"

AC_DEFINE_UNQUOTED(TEMPI_PLUGINS_DIR,["tempi-$TEMPI_API_VERSION"],[Path to Tempi plugins])
AC_DEFINE_UNQUOTED(TEMPI_API_VERSION,["$TEMPI_API_VERSION"],[Tempi API version])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
    then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

# Special cases, copied from SPIN
case "$host" in *-darwin* | *-macos10*)
  #AC_LANG_DEFINES_PROVIDED # remove autogen.sh warnings on OSX
  have_osx=true
  AM_CONDITIONAL([HAVE_OSX], true)
  AC_DEFINE(HAVE_OSX,[1],[Apple Mac OS X operating system detected])
  AC_MSG_NOTICE([Apple Mac OS X operating system detected])
  LIBTOOL="glibtool"
  SRCDIR=`pwd`
  #LIBTOOL="libtool"
  CPPFLAGS="$CPPFLAGS -D__Darwin"
  LDFLAGS="-undefined dynamic_lookup -Wl,-headerpad_max_install_names"
  if test -d /opt/local ; then
    CPPFLAGS="$CPPFLAGS -I/usr/local/include -I/opt/local/include"
    LDFLAGS="$LDFLAGS -L/usr/local/lib -L/opt/local/lib"
  elif test -d /sw ; then
    CPPFLAGS="$CPPFLAGS -I/sw/include"
    LDFLAGS="$LDFLAGS -L/sw/lib"
  fi
  # OSX does not include /usr/local/lib/pkgconfig path, so we export it here
  # (for everyone, just in case)
  # TODO: also need to add the path for linux64
  PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
  export PKG_CONFIG_PATH
  ;;
*)
  AM_CONDITIONAL([HAVE_OSX], false)
  dnl : evil:
  dnl AC_DEFINE(HAVE_OSX,[0],[Apple Mac OS X operating system not detected])
  AC_MSG_NOTICE([You are not on Apple Mac OS X.])
  ;;
esac

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for libraries
AC_CHECK_LIB([m],[sqrt])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h stddef.h stdlib.h string.h sys/time.h])

AC_CHECK_LIB([v8],[sqrt], have_v8=true, have_v8=false)
if test "x${have_v8}" = "xfalse" ; then
    AC_MSG_WARN([missing v8 library, install libv8-dev])
else
    V8_CFLAGS=""
    AC_SUBST([V8_CFLAGS], "$V8_CFLAGS")
    V8_LIBS="-lv8"
    AC_SUBST([V8_LIBS], "$V8_LIBS")
    AC_DEFINE(HAVE_V8,[1],[v8 headers successfully detected])
fi


dnl # v8
dnl AC_CHECK_HEADERS([v8.h], have_v8_headers=true, have_v8_headers=false,
dnl [#ifdef HAVE_V8_H
dnl # include <v8.h>
dnl #endif
dnl ])
dnl if test "x${have_v8_headers}" = "xfalse" ; then
dnl     AC_MSG_WARN([missing package: libv8-dev])
dnl else
dnl     V8_CFLAGS=""
dnl     AC_SUBST([V8_CFLAGS], "$V8_CFLAGS")
dnl     V8_LIBS="-lv8"
dnl     AC_SUBST([V8_LIBS], "$V8_LIBS")
dnl     AC_DEFINE(HAVE_V8,[1],[v8 headers successfully detected])
dnl fi


# portmidi
AC_CHECK_HEADERS([portmidi.h porttime.h], have_portmidi_headers=true, have_portmidi_headers=false)
if test "x${have_portmidi_headers}" = "xfalse" ; then
    AC_MSG_ERROR([missing portmidi headers, install libportmidi-dev])
fi
dnl PORTMIDI: commented out since it doesn't work on GNU/Linux!
dnl AC_CHECK_LIB([portmidi],[Pm_Initialize], have_portmidi=true, have_portmidi=false)
dnl if test "x${have_portmidi}" = "xfalse" ; then
dnl     AC_MSG_ERROR([missing portmidi library, install libportmidi-dev])
dnl fi

dnl PORTTIME: commented out since it doesn't work on Mac OS X
dnl since the mac port package is missing the binary for it.

dnl AC_CHECK_LIB([porttime],[Pt_Time], have_porttime=true, have_porttime=false)
dnl if test "x${have_porttime}" = "xfalse" ; then
dnl     AC_MSG_ERROR([missing porttime library, install libportmidi-dev])
dnl fi

if test "x${have_osx}" = "xtrue" ; then
  PORTMIDI_CFLAGS=""
  PORTMIDI_LIBS=""
else
  PORTMIDI_CFLAGS="-DPMALSA"
  PORTMIDI_LIBS="-lasound"
fi
AC_SUBST([PORTMIDI_CFLAGS], "$PORTMIDI_CFLAGS")
#PORTMIDI_LIBS="-lportmidi -lm -lasound"
PORTMIDI_LIBS="$PORTMIDI_LIBS -lportmidi -lm"
dnl -lporttime -lpthread 
AC_SUBST([PORTMIDI_LIBS], "$PORTMIDI_LIBS")

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([gettimeofday memmove mkdir setenv sqrt strchr strpbrk uname])

dnl AX_PTHREAD

# Glib
PKG_CHECK_MODULES([GLIB], [glib-2.0 gthread-2.0 gmodule-2.0], have_glib=true, have_glib=false)
if test "x${have_glib}" = "xfalse" ; then
    AC_MSG_WARN([missing package: libglib-2.0-dev])
else
    AC_DEFINE(HAVE_GLIB,[1],[Glib headers successfully detected])
fi

# Clutter
PKG_CHECK_MODULES([CLUTTER], [clutter-1.0 >= 1.0], have_clutter=true, have_clutter=false)
if test "x${have_clutter}" = "xfalse" ; then
    AC_MSG_WARN([missing package: libclutter-1.0-dev])
else
    AC_DEFINE(HAVE_CLUTTER,[1],[Clutter headers successfully detected])
fi

# Lua
PKG_CHECK_MODULES([LUA], [lua5.2], have_lua=true, have_lua=false)
if test "x${have_lua}" = "xfalse" ; then
    AC_MSG_WARN([missing package: liblua5.2-dev])
else
    AC_DEFINE(HAVE_LUA,[1],[Lua headers successfully detected])
fi

# libxml2
PKG_CHECK_MODULES([LIBXML], [libxml-2.0], have_libxml=true, have_libxml=false)
if test "x${have_libxml}" = "xfalse" ; then
    AC_MSG_ERROR([missing package: libxml2-dev])
fi

# log4cpp
PKG_CHECK_MODULES([LOG4CPP], [log4cpp], have_log4cpp=true, have_log4cpp=false)
if test "x${have_log4cpp}" = "xfalse" ; then
    AC_MSG_ERROR([missing package: liblog4cpp5-dev])
fi

PKG_CHECK_MODULES([SPATOSC], [spatosc-0.4], have_spatosc=true, have_spatosc=false)
if test "x${have_spatosc}" = "xfalse" ; then
    AC_MSG_WARN([missing package: libspatosc-dev])
else
    AC_DEFINE(HAVE_SPATOSC,[1],[SpatOSC headers successfully detected])
fi

PKG_CHECK_MODULES([ATOM], [atom-0.2], have_atom=true, have_atom=false)
if test "x${have_atom}" = "xfalse" ; then
    AC_MSG_ERROR([missing package: libatom-dev])
fi

PKG_CHECK_MODULES([LIBMAPPER], [libmapper-0], have_libmapper=true, have_libmapper=false)
if test "x${have_libmapper}" = "xfalse" ; then
    AC_MSG_WARN([missing package: libmapper-dev])
else
    AC_MSG_NOTICE([Libmapper headers successfully detected but we will not use them for now])
    dnl AC_DEFINE(HAVE_LIBMAPPER,[1],[Libmapper headers successfully detected])
fi

dnl # BOOST
dnl AX_BOOST_BASE([1.35])
dnl 
dnl AX_BOOST_SIGNALS
dnl if test "x${ax_cv_boost_signals}" = "xno" ; then
dnl 	AC_MSG_ERROR([missing package: libboost-signals-dev])
dnl fi

PKG_CHECK_MODULES(LIBLO, liblo, have_liblo=true, have_liblo=false)
if test "x${have_liblo}" = "xfalse" ; then
    AC_MSG_ERROR([Missing liblo])
fi

dnl AX_PYTHON
dnl dnl AM_PYTHON_PATH
dnl PYTHON_CPPFLAGS="`python-config --cflags`"
dnl PYTHON_CPPFLAGS=${PYTHON_CPPFLAGS//"-Wstrict-prototypes"/}
dnl PYTHON_LDFLAGS="`python-config --ldflags`"
dnl AC_SUBST(PYTHON_CPPFLAGS)
dnl AC_SUBST(PYTHON_LDFLAGS)

BOOST_REQUIRE([1.35])
BOOST_DATE_TIME
BOOST_FILESYSTEM
BOOST_PROGRAM_OPTIONS
dnl BOOST_PYTHON
BOOST_REGEX
BOOST_SIGNALS
BOOST_SYSTEM
BOOST_THREADS

dnl AC_MSG_NOTICE([BOOST_LDFLAGS:              ${BOOST_LDFLAGS}])
AC_MSG_NOTICE([BOOST_CPPFLAGS:             ${BOOST_CPPFLAGS}])
AC_MSG_NOTICE([BOOST_DATE_TIME_LIBS:       ${BOOST_DATE_TIME_LIBS}])
AC_MSG_NOTICE([BOOST_FILESYSTEM_LIBS:      ${BOOST_FILESYSTEM_LIBS}])
AC_MSG_NOTICE([BOOST_PROGRAM_OPTIONS_LIBS: ${BOOST_PROGRAM_OPTIONS_LIBS}])
dnl AC_MSG_NOTICE([BOOST_PYTHON_LDFLAGS:       ${BOOST_PYTHON_LDFLAGS}])
dnl AC_MSG_NOTICE([BOOST_PYTHON_LIBS:          ${BOOST_PYTHON_LIBS}])
AC_MSG_NOTICE([BOOST_SIGNALS_LIBS:         ${BOOST_SIGNALS_LIBS}])
AC_MSG_NOTICE([BOOST_SYSTEM_LIBS:          ${BOOST_SYSTEM_LIBS}])
AC_MSG_NOTICE([BOOST_THREAD_LIBS:         ${BOOST_THREAD_LIBS}])

CFLAGS+=" -fPIC -Wall -g3 -O0" dnl -Wextra -Wfatal-errors -Werror "

# GNU help2man creates man pages from --help output; in many cases, this
# is sufficient, and obviates the need to maintain man pages separately.
# However, this means invoking executables, which we generally cannot do
# when cross-compiling, so we test to avoid that (the variable
# "cross_compiling" is set by AC_PROG_CC).
if test $cross_compiling = no; then
    AM_MISSING_PROG(HELP2MAN, help2man)
else
    HELP2MAN=:
fi

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([
        Makefile 
        tempi/tempi-version.h
        tempi/Makefile 
        tempi/tests/Makefile
        plugins-base/Makefile
        plugins-base/tests/Makefile
        python_module/Makefile
        src/Makefile
        src/tempi-midi-looper/Makefile
        src/tempi-osc-forward/Makefile
        src/tempi-draw/Makefile
        src/tempi-inspect/Makefile
        src/tempi-launch/Makefile
        src/tempi-osc-looper/Makefile
        doxyfile 
        data/Makefile
        doc/Makefile
        man/Makefile
        abstractions/Makefile
        tempi-${TEMPI_API_VERSION}.pc:tempi.pc.in
        ])

AC_OUTPUT


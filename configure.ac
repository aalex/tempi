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
m4_define([tempi_minor_version], [1])
m4_define([tempi_micro_version], [4])

m4_define([tempi_release_status],
          [m4_if(m4_eval(tempi_micro_version % 2), [1], [git],
                 [m4_if(m4_eval(tempi_minor_version % 2), [1], [snapshot],
                                                                 [release])])])

m4_define([tempi_version], [tempi_major_version.tempi_minor_version.tempi_micro_version])

# change this only when breaking the API
m4_define([tempi_api_version], [0.1])

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
        [http://github.com/aalex/tempi/issues],
        [tempi],
        [http://tempi.toonloop.com])
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

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for libraries
AC_CHECK_LIB([m],[sqrt])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h stddef.h stdlib.h string.h sys/time.h])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([gettimeofday memmove mkdir setenv sqrt strchr strpbrk uname])

AX_PTHREAD

# STK
# TODO: print some message when configuring STK is OK
AC_LANG_PUSH(C++)
# save original flags
SAVED_LIBS=$LIBS
LIBS="$LIBS -lstk"
AC_LINK_IFELSE(
        [AC_LANG_PROGRAM([ #include <stk/RtMidi.h> ],
        [RtMidiIn input])],
        [AC_SUBST(STK_LIBS," -lstk -lrt -lm")],
        [AC_MSG_ERROR([libstk is not installed: alsa])])
LIBS=$SAVED_LIBS # restore them
AC_SUBST(STK_CXXFLAGS, "-D__LINUX_ALSA__ -D__UNIX_JACK__ -D__LITTLE_ENDIAN__")
AC_LANG_POP(C++)

# Glib
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.10.2], have_glib=true, have_glib=false)
if test "x${have_glib}" = "xfalse" ; then
    AC_MSG_ERROR([missing package: libglib-2.0-dev])
fi
AC_SUBST([GLIB_LIBS])
AC_SUBST([GLIB_CFLAGS])

# Clutter
PKG_CHECK_MODULES([CLUTTER], [clutter-1.0 >= 1.0], have_clutter=true, have_clutter=false)
if test "x${have_clutter}" = "xfalse" ; then
    AC_MSG_ERROR([missing package: libclutter-1.0-dev])
fi
AC_SUBST([CLUTTER_LIBS])
AC_SUBST([CLUTTER_CFLAGS])

# libxml2
PKG_CHECK_MODULES([LIBXML], [libxml-2.0], have_libxml=true, have_libxml=false)
if test "x${have_libxml}" = "xfalse" ; then
    AC_MSG_ERROR([missing package: libxml2-dev])
fi
AC_SUBST([LIBXML_LIBS])
AC_SUBST([LIBXML_CFLAGS])

# BOOST
AX_BOOST_BASE([1.35])

AX_BOOST_SIGNALS
if test "x${ax_cv_boost_signals}" = "xno" ; then
	AC_MSG_ERROR([missing package: libboost-signals-dev])
fi

#Set LIBLO with pkg-config
PKG_CHECK_MODULES(LIBLO, liblo, have_liblo=true, have_liblo=false)
if test "x${have_liblo}" = "xfalse" ; then
    AC_MSG_ERROR([Missing liblo])
fi

AX_BOOST_PROGRAM_OPTIONS
if test "x${ax_cv_boost_program_options}" = "xno" ; then
	AC_MSG_ERROR([missing package: libboost-program-options-dev])
fi

dnl AX_BOOST_FILESYSTEM
dnl if test "x${ax_cv_boost_filesystem}" = "xno" ; then
dnl 	AC_MSG_ERROR([missing package: libboost-filesystem-dev])
dnl fi

AX_BOOST_THREAD
if test "x${ax_cv_boost_thread}" = "xno" ; then
    AC_MSG_ERROR([missing package: libboost-thread-dev])
fi

dnl AX_BOOST_DATE_TIME
dnl if test "x${ax_cv_boost_date_time}" = "xno" ; then
dnl 	AC_MSG_ERROR([missing package: libboost-date-time-dev])
dnl fi
dnl 
dnl AX_BOOST_SYSTEM
dnl if test "x${ax_cv_boost_system}" = "xno" ; then
dnl 	AC_MSG_ERROR([missing package: libboost-system-dev])
dnl fi

CFLAGS+=" -Wall -Wextra -Wfatal-errors -Werror "

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

dnl TODO: check for Boost

# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES([
        Makefile 
        tempi/tempi-version.h
        tempi/Makefile 
        src/Makefile
        src/midilooper/Makefile
        src/osc-forward/Makefile
        src/tempi-draw/Makefile
        tests/Makefile
        doxyfile 
        data/Makefile
        doc/Makefile
        man/Makefile
        tempi-${TEMPI_API_VERSION}.pc:tempi.pc.in
        ])

AC_OUTPUT


dnl Checks for STK
dnl Authors: Alexandre Quessy, Tristan Matthews

AC_DEFUN([AC_STK], [
    AC_MSG_CHECKING([for STK])
    AC_LANG_PUSH(C++)
    AC_LINK_IFELSE(
        [
            AC_LANG_PROGRAM([#include <stk/RtMidi.h>],
            [RtMidiIn *input])
        ],
        [
            AC_SUBST(STK_LIBS," -lstk -lrt -lm")
            AC_MSG_RESULT([ok])
            AC_SUBST(STK_CXXFLAGS, [-D__LINUX_ALSA__ -D__UNIX_JACK__])
            AC_SUBST(STK_LIBS, [-lstk -L/usr/local/lib])
        ],
        [
            AC_MSG_RESULT([failed])
            AC_MSG_ERROR([libstk is not installed])
        ])
    AC_LANG_POP(C++)
    ])

dnl AC_DEFINE(HAVE_STK,[1],[STK headers successfully detected])

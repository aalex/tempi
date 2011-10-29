#ifndef __LEGACY_H__
#define __LEGACY_H__

#include <clutter/clutter.h>

#if CLUTTER_CHECK_VERSION(1,4,0)
#else
#define CLUTTER_KEY_Down CLUTTER_Down
#define CLUTTER_KEY_Escape CLUTTER_Escape
#define CLUTTER_KEY_F1 CLUTTER_F1
#define CLUTTER_KEY_Left CLUTTER_Left
#define CLUTTER_KEY_Return CLUTTER_Return
#define CLUTTER_KEY_Right CLUTTER_Right
#define CLUTTER_KEY_Tab CLUTTER_Tab
#define CLUTTER_KEY_Up CLUTTER_Up
#define CLUTTER_KEY_q CLUTTER_q
#define CLUTTER_KEY_space CLUTTER_space
#define CLUTTER_KEY_Delete CLUTTER_Delete
#define CLUTTER_KEY_BackSpace CLUTTER_BackSpace
#endif

#endif // ifndef

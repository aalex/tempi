/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Some macros and constants.
 */

#ifndef __MILLER_MACROS_H__
#define __MILLER_MACROS_H__

#include <clutter/clutter.h>

// Clutter legacy macro aliases:
#if CLUTTER_CHECK_VERSION(1,4,0)
#else
#define CLUTTER_KEY_Down CLUTTER_Down
#define CLUTTER_KEY_Escape CLUTTER_Escape
#define CLUTTER_KEY_F1 CLUTTER_F1
#define CLUTTER_KEY_F2 CLUTTER_F2
#define CLUTTER_KEY_F3 CLUTTER_F3
#define CLUTTER_KEY_F4 CLUTTER_F4
#define CLUTTER_KEY_F11 CLUTTER_F11
#define CLUTTER_KEY_Left CLUTTER_Left
#define CLUTTER_KEY_Return CLUTTER_Return
#define CLUTTER_KEY_Right CLUTTER_Right
#define CLUTTER_KEY_Tab CLUTTER_Tab
#define CLUTTER_KEY_Up CLUTTER_Up
#define CLUTTER_KEY_q CLUTTER_q
#define CLUTTER_KEY_s CLUTTER_s
#define CLUTTER_KEY_space CLUTTER_space
#define CLUTTER_KEY_Delete CLUTTER_Delete
#define CLUTTER_KEY_BackSpace CLUTTER_BackSpace
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

namespace miller {

// Color constants:
static const ClutterColor BLACK = { 0x00, 0x00, 0x00, 0xff };
static const ClutterColor BLUE = { 0x33, 0x33, 0xcc, 0xff };
static const ClutterColor CYAN = { 0x33, 0xcc, 0xcc, 0xff };
static const ClutterColor GRAY_LIGHT = { 0xcc, 0xcc, 0xcc, 0xff };
static const ClutterColor GRAY_MEDIUM = { 0x99, 0x99, 0x99, 0xff };
static const ClutterColor GREEN = { 0x33, 0xcc, 0x33, 0xff };
static const ClutterColor MAGENTA = { 0xcc, 0x33, 0xcc, 0xff };
static const ClutterColor RED = { 0xcc, 0x33, 0x33, 0xff };
static const ClutterColor WHITE = { 0xff, 0xff, 0xff, 0xff };
static const ClutterColor YELLOW = { 0xcc, 0xcc, 0x33, 0xff };

} // end of namespace

#endif // ifndef


/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * @file The MillerConnection GOject class - a child of ClutterActor.
 */
#ifndef __MILLER_CONNECTION_H__
#define __MILLER_CONNECTION_H__

#include <glib-object.h>
#include <clutter/clutter.h>

G_BEGIN_DECLS

#define MILLER_TYPE_CONNECTION            (miller_connection_get_type ())
#define MILLER_CONNECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), MILLER_TYPE_CONNECTION, MillerConnection))
#define MILLER_IS_CONNECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MILLER_TYPE_CONNECTION))
#define MILLER_CONNECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MILLER_TYPE_CONNECTION, MillerConnectionClass))
#define MILLER_IS_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MILLER_TYPE_CONNECTION))
#define MILLER_CONNECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), MILLER_TYPE_CONNECTION, MillerConnectionClass))

typedef struct _MillerConnectionPrivate MillerConnectionPrivate;

typedef struct _MillerConnection
{
  ClutterActor parent_instance;
  MillerConnectionPrivate *priv;
} MillerConnection;

typedef struct _MillerConnectionClass
{
  ClutterActorClass parent_class;
} MillerConnectionClass;

GType miller_connection_get_type (void) G_GNUC_CONST;

// ClutterActor *miller_connection_new              (void);
ClutterActor *miller_connection_new_full   (const ClutterColor *color,
                          guint stroke_width,
                          gfloat x1, gfloat y1, gfloat x2, gfloat y2);

// void          miller_connection_get_color        (MillerConnection   *actor,
//                                                   ClutterColor       *color);
void          miller_connection_set_color        (MillerConnection   *actor,
						  const ClutterColor *color);
// guint         miller_connection_get_stroke_width (MillerConnection   *actor);
void          miller_connection_set_stroke_width (MillerConnection   *actor,
                                                   guint               width);
// gfloat        miller_connection_get_x1(MillerConnection *actor);
// gfloat        miller_connection_get_y1(MillerConnection *actor);
// gfloat        miller_connection_get_x2(MillerConnection *actor);
// gfloat        miller_connection_get_y2(MillerConnection *actor);
// 
void        miller_connection_set_x1(MillerConnection *actor, gfloat value);
void        miller_connection_set_y1(MillerConnection *actor, gfloat value);
void        miller_connection_set_x2(MillerConnection *actor, gfloat value);
void        miller_connection_set_y2(MillerConnection *actor, gfloat value);

G_END_DECLS

#endif // ifndef


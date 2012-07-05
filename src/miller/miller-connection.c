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

#include "miller-connection.h" 
#include "cogl/cogl.h"

// We need to define some stuff that we can get from clutter-private.h:
#ifndef CLUTTER_PARAM_READWRITE 
#define CLUTTER_PARAM_READWRITE \
        G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |G_PARAM_STATIC_BLURB
#endif

G_DEFINE_TYPE (MillerConnection, miller_connection, CLUTTER_TYPE_ACTOR);

enum
{
  PROP_0,

  PROP_COLOR,
  PROP_STROKE_WIDTH,
  PROP_X1,
  PROP_Y1,
  PROP_X2,
  PROP_Y2
};

#define MILLER_CONNECTION_GET_PRIVATE(obj) \
(G_TYPE_INSTANCE_GET_PRIVATE ((obj), MILLER_TYPE_CONNECTION, MillerConnectionPrivate))

struct _MillerConnectionPrivate
{
  ClutterColor color;
  guint stroke_width;
  gfloat x1;
  gfloat y1;
  gfloat x2;
  gfloat y2;
};

static const ClutterColor default_color        = { 255, 255, 255, 255 };
static const guint default_stroke_width = 1;

static void
miller_connection_finalize (GObject *object)
{
  G_OBJECT_CLASS (miller_connection_parent_class)->finalize (object);
}

static void
miller_connection_dispose (GObject *object)
{
  G_OBJECT_CLASS (miller_connection_parent_class)->dispose (object);
}

static void
miller_connection_set_property (GObject      *object,
				guint         prop_id,
				const GValue *value,
				GParamSpec   *pspec)
{
  MillerConnection *connection = MILLER_CONNECTION(object);

  switch (prop_id)
    {
    case PROP_COLOR:
      miller_connection_set_color (connection, clutter_value_get_color (value));
      break;
    case PROP_STROKE_WIDTH:
      miller_connection_set_stroke_width (connection,
                                          g_value_get_uint (value));
      break;
    case PROP_X1:
      miller_connection_set_x1 (connection, g_value_get_float (value));
      break;
    case PROP_Y1:
      miller_connection_set_y1 (connection, g_value_get_float (value));
      break;
    case PROP_X2:
      miller_connection_set_x2 (connection, g_value_get_float (value));
      break;
    case PROP_Y2:
      miller_connection_set_y2 (connection, g_value_get_float (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

void
miller_connection_set_stroke_width (MillerConnection *connection,
                                    guint             width)
{
  MillerConnectionPrivate *priv;
  g_return_if_fail (MILLER_IS_CONNECTION (connection));
  priv = connection->priv;
  if (priv->stroke_width != width)
    {
      g_object_ref (connection);
      priv->stroke_width = width;
      clutter_actor_queue_redraw (CLUTTER_ACTOR (connection));
      g_object_notify (G_OBJECT (connection), "stroke-width");
      g_object_unref (connection);
    }
}

void        miller_connection_set_x1(MillerConnection *connection, gfloat value)
{
  MillerConnectionPrivate *priv;
  g_return_if_fail (MILLER_IS_CONNECTION (connection));
  priv = connection->priv;
  if (priv->x1 != value)
    {
      g_object_ref (connection);
      priv->x1 = value;
      clutter_actor_queue_redraw (CLUTTER_ACTOR (connection));
      g_object_notify (G_OBJECT (connection), "x1");
      g_object_unref (connection);
    }
}
void        miller_connection_set_y1(MillerConnection *connection, gfloat value)
{
  MillerConnectionPrivate *priv;
  g_return_if_fail (MILLER_IS_CONNECTION (connection));
  priv = connection->priv;
  if (priv->y1 != value)
    {
      g_object_ref (connection);
      priv->y1 = value;
      clutter_actor_queue_redraw (CLUTTER_ACTOR (connection));
      g_object_notify (G_OBJECT (connection), "y1");
      g_object_unref (connection);
    }
}
void        miller_connection_set_x2(MillerConnection *connection, gfloat value)
{
  MillerConnectionPrivate *priv;
  g_return_if_fail (MILLER_IS_CONNECTION (connection));
  priv = connection->priv;
  if (priv->x2 != value)
    {
      g_object_ref (connection);
      priv->x2 = value;
      clutter_actor_queue_redraw (CLUTTER_ACTOR (connection));
      g_object_notify (G_OBJECT (connection), "x2");
      g_object_unref (connection);
    }
}
void        miller_connection_set_y2(MillerConnection *connection, gfloat value)
{
  MillerConnectionPrivate *priv;
  g_return_if_fail (MILLER_IS_CONNECTION (connection));
  priv = connection->priv;
  if (priv->y2 != value)
    {
      g_object_ref (connection);
      priv->y2 = value;
      clutter_actor_queue_redraw (CLUTTER_ACTOR (connection));
      g_object_notify (G_OBJECT (connection), "y2");
      g_object_unref (connection);
    }
}

void
miller_connection_set_color (MillerConnection   *connection,
			     const ClutterColor *color)
{
  MillerConnectionPrivate *priv;

  g_return_if_fail (MILLER_IS_CONNECTION(connection));
  g_return_if_fail (color != NULL);

  g_object_ref (connection);

  priv = connection->priv;

  priv->color.red = color->red;
  priv->color.green = color->green;
  priv->color.blue = color->blue;
  priv->color.alpha = color->alpha;

  clutter_actor_queue_redraw (CLUTTER_ACTOR (connection));

  g_object_notify (G_OBJECT (connection), "color");
  g_object_unref (connection);
}

static void
miller_connection_get_property (GObject    *object,
				guint       prop_id,
				GValue     *value,
				GParamSpec *pspec)
{
  MillerConnectionPrivate *priv = MILLER_CONNECTION(object)->priv;

  switch (prop_id)
    {
    case PROP_COLOR:
      clutter_value_set_color (value, &priv->color);
      break;
    case PROP_STROKE_WIDTH:
      g_value_set_uint (value, priv->stroke_width);
      break;
    case PROP_X1:
      g_value_set_float (value, priv->x1);
      break;
    case PROP_Y1:
      g_value_set_float (value, priv->y1);
      break;
    case PROP_X2:
      g_value_set_float (value, priv->x2);
      break;
    case PROP_Y2:
      g_value_set_float (value, priv->y2);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
miller_connection_paint (ClutterActor *self)
{
  MillerConnectionPrivate *priv = MILLER_CONNECTION (self)->priv;
  //ClutterGeometry geom;
  guint8 tmp_alpha;

  //clutter_actor_get_allocation_geometry (self, &geom);

  /* parent paint call will have translated us into position so
   * paint from 0, 0
   */
  /* compute the composited opacity of the actor taking into
   * account the opacity of the color set by the user
   */
  tmp_alpha = clutter_actor_get_paint_opacity (self)
            * priv->color.alpha
            / 255;


  //cogl_push_matrix();

  /* paint the border */
  cogl_set_source_color4ub (priv->color.red,
                            priv->color.green,
                            priv->color.blue,
                            tmp_alpha);

  gfloat x1 = priv->x1;
  gfloat y1 = priv->y1;
  gfloat x2 = priv->x2;
  gfloat y2 = priv->y2;

  cogl_path_line(x1, y1, x2, y2);
  cogl_path_stroke();

  //cogl_pop_matrix();
}

static void
miller_connection_class_init (MillerConnectionClass *klass)
{
  GObjectClass      *gobject_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);
  GParamSpec        *pspec;

  actor_class->paint            = miller_connection_paint;
  //TODO actor_class->get_paint_volume = miller_connection_get_paint_volume;
  //TODO actor_class->has_overlaps     = miller_connection_has_overlaps;

  gobject_class->finalize     = miller_connection_finalize;
  gobject_class->dispose      = miller_connection_dispose;
  gobject_class->set_property = miller_connection_set_property;
  gobject_class->get_property = miller_connection_get_property;

  /**
   * MillerConnection:color:
   *
   * The color of the stroke.
   */
  pspec = clutter_param_spec_color ("color",
                                    "Color",
                                    "The color of the stroke",
                                    &default_color,
                                    CLUTTER_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_COLOR, pspec);


  /**
   * MillerConnection:stroke-width:
   *
   * The width of the stroke of the connection, in pixels.
   */
  g_object_class_install_property (gobject_class,
                                   PROP_STROKE_WIDTH,
                                   g_param_spec_uint ("stroke-width",
                                                      "Stroke Width",
                                                      "The width of the stroke of the connection",
                                                      0, G_MAXUINT,
                                                      0,
                                                      CLUTTER_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_X1,
                                   g_param_spec_uint ("x1", "x1", "x1",
                                                      G_MINFLOAT, G_MAXFLOAT,
                                                      0.0f,
                                                      CLUTTER_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_Y1,
                                   g_param_spec_uint ("y1", "y1", "y1",
                                                      G_MINFLOAT, G_MAXFLOAT,
                                                      0.0f,
                                                      CLUTTER_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_X2,
                                   g_param_spec_uint ("x2", "x2", "x2",
                                                      G_MINFLOAT, G_MAXFLOAT,
                                                      0.0f,
                                                      CLUTTER_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_Y2,
                                   g_param_spec_uint ("y2", "y2", "y2",
                                                      G_MINFLOAT, G_MAXFLOAT,
                                                      0.0f,
                                                      CLUTTER_PARAM_READWRITE));

  g_type_class_add_private (gobject_class, sizeof (MillerConnectionPrivate));
}

static void
miller_connection_init (MillerConnection *self)
{
  MillerConnectionPrivate *priv;

  self->priv = priv = MILLER_CONNECTION_GET_PRIVATE (self);

  priv->color = default_color;
  priv->stroke_width = default_stroke_width;
}

ClutterActor *
miller_connection_new_full (const ClutterColor *color,
                          guint stroke_width,
                          gfloat x1, gfloat y1, gfloat x2, gfloat y2)
{
  return g_object_new (MILLER_TYPE_CONNECTION,
		       "color", color,
               "stroke_width", stroke_width,
               "x1", x1,
               "y1", y1,
               "x2", x2,
               "y2", y2,
		       NULL);
}



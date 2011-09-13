#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator()
{
    random_generator_ = g_rand_new();
    group_ = clutter_group_new();
}

ClutterActor *ParticleGenerator::getRoot()
{
    return group_;
}

static gboolean delete_the_actor(gpointer user_data)
{
    ClutterActor *actor = (ClutterActor *) user_data;
}

static void on_animation_completed(ClutterAnimation *animation, ClutterActor *actor)
{
    g_idle_add(delete_the_actor, actor);
}

void ParticleGenerator::onDraw()
{
    // adds a new actor on each frame
    ClutterColor color = 
    { 
        g_rand_int_range(random_generator_, 100, 255), 
        g_rand_int_range(random_generator_, 50, 200), 
        g_rand_int_range(random_generator_, 0, 50), 
        g_rand_int_range(random_generator_, 100, 200)
    };
    ClutterActor *particle = clutter_rectangle_new_with_color(&color);
    clutter_container_add_actor(CLUTTER_CONTAINER(group_), particle);
    clutter_actor_set_position(particle, 0.0, 0.0);
    double size = g_rand_double_range(random_generator_, 1.0, 40.0);
    gint duration = g_rand_int_range(random_generator_, 100, 1000);
    clutter_actor_set_size(particle, size, size);
    clutter_actor_set_opacity(particle, 255);
    ClutterAnimation *animation = clutter_actor_animate(particle, CLUTTER_LINEAR, duration, 
        "x", (float) g_rand_double_range(random_generator_, 0.0, 200.0),
        "y", (float) g_rand_double_range(random_generator_, 0.0, 200.0),
        "width", 0.0f,
        "height", 0.0f,
        "opacity", 0,
        "signal::completed", on_animation_completed, particle,
        NULL);
}


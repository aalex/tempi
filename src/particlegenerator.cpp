#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator()
{
    random_generator_ = g_rand_new();
    group_ = clutter_group_new();
    ClutterColor color = { 255, 255, 255, 255 };
    for (int i = 0; i < NUM_PARTICLE_PER_SAMPLER; ++i)
    {
        ClutterActor *particle = clutter_rectangle_new_with_color(&color);
        clutter_container_add_actor(CLUTTER_CONTAINER(group_), particle);
        clutter_actor_hide(particle);
        actors_.push_back(particle);
    }
    current_particle_ = 0;
}

ClutterActor *ParticleGenerator::getRoot()
{
    return group_;
}

static gboolean hide_the_actor(gpointer user_data)
{
    ClutterActor *actor = (ClutterActor *) user_data;
    clutter_actor_hide(actor);
    return TRUE;
}

static void on_animation_completed(ClutterAnimation *animation, ClutterActor *actor)
{
    g_idle_add(hide_the_actor, actor);
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
    ClutterActor *particle = actors_[current_particle_];
    current_particle_ = (current_particle_ + 1) % NUM_PARTICLE_PER_SAMPLER;
    clutter_actor_show(particle);
    clutter_rectangle_set_color(CLUTTER_RECTANGLE(particle), &color);
    clutter_actor_set_position(particle, source_x_, source_y_);
    double size = g_rand_double_range(random_generator_, 1.0, 20.0);
    gint duration = g_rand_int_range(random_generator_, 500, 1500);
    clutter_actor_set_size(particle, size, size);
    clutter_actor_set_opacity(particle, 255);
    clutter_actor_set_anchor_point_from_gravity(particle, CLUTTER_GRAVITY_CENTER);
    clutter_actor_set_z_rotation_from_gravity(particle, g_rand_double_range(random_generator_, -180.0, 180.0), CLUTTER_GRAVITY_CENTER);
    ClutterAnimation *animation = clutter_actor_animate(particle, CLUTTER_EASE_OUT_QUAD, duration, 
        "x", (float) g_rand_double_range(random_generator_, source_x_ - 100.0, source_x_ + 100.0),
        "y", (float) g_rand_double_range(random_generator_, source_y_ - 100.0, source_y_ + 100.0),
        "width", 0.0f,
        "height", 0.0f,
        "opacity", 0,
        "rotation-angle-z", g_rand_double_range(random_generator_, -180.0, 180.0),
        "signal::completed", on_animation_completed, particle,
        NULL);
}

void ParticleGenerator::setSourcePosition(double x, double y)
{
    source_x_ = x;
    source_y_ = y;
}



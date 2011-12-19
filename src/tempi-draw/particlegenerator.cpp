#include "particlegenerator.h"

static void set_blending_mode_for_texture(ClutterTexture *texture, const gchar *blend)
{
    CoglHandle material = clutter_texture_get_cogl_material(texture);
    GError *error = NULL;
#if CLUTTER_CHECK_VERSION(1, 4, 0)
    gboolean success = cogl_material_set_blend(COGL_MATERIAL(material), blend, &error);
#else
    gboolean success = cogl_material_set_blend(material, blend, &error);
#endif

    if (error)
    {
        g_critical("Error setting blend: %s\n", error->message);
        g_error_free(error);
    }
    if (! success)
    {
        g_critical("Could not set blend mode");
    }
}

static void set_additive_blending_mode_for_texture(ClutterTexture *texture, bool additive)
{
    static const gchar *NORMAL_BLEND_MODE = "RGBA = ADD (SRC_COLOR * (SRC_COLOR[A]), DST_COLOR * (1-SRC_COLOR[A]))";
    static const gchar *ADD_BLEND_MODE = "RGBA = ADD (SRC_COLOR * (SRC_COLOR[A]), DST_COLOR)";
    if (additive)
        set_blending_mode_for_texture(texture, ADD_BLEND_MODE);
    else
        set_blending_mode_for_texture(texture, NORMAL_BLEND_MODE);
}

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

void ParticleGenerator::setColor(int r, int g, int b)
{
    color_.red = r;
    color_.green = g;
    color_.blue = b;
}

ClutterActor *ParticleGenerator::getRoot()
{
    return group_;
}

static gboolean hide_the_actor(gpointer user_data)
{
    ClutterActor *actor = (ClutterActor *) user_data;
    if (CLUTTER_IS_ACTOR(actor))
        clutter_actor_hide(actor);
    return TRUE;
}

static void on_animation_completed(ClutterAnimation *animation, ClutterActor *actor)
{
    g_idle_add(hide_the_actor, actor);
}

void ParticleGenerator::onDraw()
{
    for (int i = 0; i < NUM_PARTICLE_PER_FRAME; ++i)
    {
        generateOneParticle();
    }
}

void ParticleGenerator::generateOneParticle()
{
    int r = color_.red;
    int g = color_.green;
    int b = color_.blue;
    int a = 200;
    int delta = 17;
    ClutterColor color = 
    { 
        g_rand_int_range(random_generator_, r - delta, r + delta),
        g_rand_int_range(random_generator_,  g - delta, g + delta),
        g_rand_int_range(random_generator_,  b - delta, b + delta),
        g_rand_int_range(random_generator_,  a - delta, a + delta)
    };
    ClutterActor *particle = actors_[current_particle_];
    current_particle_ = (current_particle_ + 1) % NUM_PARTICLE_PER_SAMPLER;
    clutter_actor_show(particle);
    clutter_rectangle_set_color(CLUTTER_RECTANGLE(particle), &color);
    clutter_actor_set_position(particle, source_x_, source_y_);
    double size = g_rand_double_range(random_generator_, 5.0, 10.0);
    gint duration = g_rand_int_range(random_generator_, 1000, 2000);
    clutter_actor_set_size(particle, size, size);
    clutter_actor_set_opacity(particle, 255);
    clutter_actor_set_anchor_point_from_gravity(particle, CLUTTER_GRAVITY_CENTER);
    clutter_actor_set_z_rotation_from_gravity(particle, g_rand_double_range(random_generator_, -180.0, 180.0), CLUTTER_GRAVITY_CENTER);
    ClutterAnimation *animation = clutter_actor_animate(particle, CLUTTER_EASE_OUT_QUAD, duration, 
        "x", (float) g_rand_double_range(random_generator_, source_x_ - MAX_DISTANCE, source_x_ + MAX_DISTANCE),
        "y", (float) g_rand_double_range(random_generator_, source_y_ - MAX_DISTANCE, source_y_ + MAX_DISTANCE),
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



#ifndef __PARTICLEGENERATOR_H__
#define __PARTICLEGENERATOR_H__

#include <clutter/clutter.h>
#include <glib.h>
#include <vector>

class Actor
{
    public:
        virtual ClutterActor *getRoot() = 0;
        virtual void onDraw() = 0;
};

class ParticleGenerator : public Actor
{
    public:
        ParticleGenerator();
        virtual ClutterActor *getRoot();
        virtual void onDraw();
        void setSourcePosition(double x, double y);
        void setColor(int r, int g, int b);
    private:
        ClutterActor *group_;
        GRand *random_generator_;
        double source_x_;
        double source_y_;
        std::vector<ClutterActor *> actors_;
        static const unsigned int NUM_PARTICLE_PER_SAMPLER = 100;
        static const unsigned int NUM_PARTICLE_PER_FRAME = 1;
        static const double MAX_DISTANCE = 25.0;
        int current_particle_;
        void generateOneParticle();
    private:
        ClutterColor color_;
};

#endif // ifndef


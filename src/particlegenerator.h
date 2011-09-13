#ifndef __PARTICLEGENERATOR_H__
#define __PARTICLEGENERATOR_H__

#include <clutter/clutter.h>
#include <glib.h>

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
    private:
        ClutterActor *group_;
        GRand *random_generator_;
};

#endif // ifndef


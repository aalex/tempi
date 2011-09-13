/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __PARTICLES_APPLICATION_H_
#define __PARTICLES_APPLICATION_H_

#include "base_application.h"
#include <tempi/tempi.h>
#include <OgreParticleSystem.h>

namespace Ogre
{
    class SceneNode;
}

class ParticlesApplication : public BaseApplication
{
    public: 
        ParticlesApplication();
    private:
        void drawAxes();
        //void createAudioScene();
        virtual bool processUnbufferedInput(const Ogre::FrameEvent& evt);
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
        virtual void createScene();
        void drawParticles();
        //Ogre::SceneNode *headNode_;
        //Ogre::SceneNode *nodeTwo_;
        Ogre::ParticleSystem *mParticleSystem;
};

#endif // #ifndef


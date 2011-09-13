/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include <signal.h>
#include "particles_application.h"
#include "text_renderer.h"
#include "tempi/tempi.h"
#include <OgreMaterial.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>

namespace {
volatile int interrupted = 0; // caught signals will be stored here
void terminateSignalHandler(int sig)
{
    interrupted = sig;
}

void attachInterruptHandlers()
{
    // attach interrupt handlers
    signal(SIGINT, &terminateSignalHandler);
    signal(SIGTERM, &terminateSignalHandler);
}

} // end anonymous namespace

static const double OSC_FLUSH_INTERVAL = 1 / 15.0; // How many seconds between position updates being sent over OSC 

ParticlesApplication::ParticlesApplication()
{
    // pass
}

bool ParticlesApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
    if (! processUnbufferedInput(evt)) 
        return false;

    return ret;
}

void ParticlesApplication::drawParticles()
{
    const float maxDist  = 250.0;
    const float mirrorDist = maxDist * 0.99;
    const float dimFactor = 0.8 * 0.005 * 0.005;
    const float maxDist2 = maxDist * maxDist;
    Ogre::Camera* cam = mCamera; // ->getCamera();
    const Ogre::Vector3& camPos = cam->getPositionForViewUpdate(); // getWorldPosition();
    Ogre::ParticleIterator pit = mParticleSystem->_getIterator();
 
    while (! pit.end())
    {
        Ogre::Particle* particle = pit.getNext();
        Ogre::Vector3& pos = particle->position;
        particle->timeToLive = 999999.0f;
        Ogre::Vector3 pDir = pos-camPos;
        float dist = pDir.squaredLength();
        float dim = dist * dimFactor;
        particle->setDimensions(dim, dim);
        if (dist > maxDist2)
        {
            pDir.normalise();
            Ogre::Vector3 p = camPos - pDir * mirrorDist;
            particle->position = p;
        } 
    }
}

bool ParticlesApplication::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
    if (interrupted)
    {
        std::cerr << "Got interrupt signal...exittting." << std::endl;  
        return false;
    }
    else if (mKeyboard == 0)
        return false;

    drawParticles();
    return true;
}

void ParticlesApplication::createScene()
{
    // particle system:
    mParticleSystem = mSceneMgr->createParticleSystem("particleSystem_0", "Examples/PurpleFountain");

    // set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.6, 0.2));

    // create a light
    Ogre::Light *l = mSceneMgr->createLight("MainLight");
    l->setPosition(20, 80, 50);

    // draw axes
    drawAxes();
}

void ParticlesApplication::drawAxes()
{
    Ogre::ManualObject* xAxis =  mSceneMgr->createManualObject("xaxis"); 
    Ogre::SceneNode* xAxisNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("xaxisNode"); 

    Ogre::MaterialPtr xAxisMaterial = Ogre::MaterialManager::getSingleton().create("xaxisMaterial","debugger"); 
    xAxisMaterial->setReceiveShadows(false); 
    xAxisMaterial->getTechnique(0)->setLightingEnabled(true); 
    xAxisMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,0,1,0); 
    xAxisMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
    xAxisMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 
    //xAxisMaterial->dispose();  // dispose pointer, not the material

    xAxis->begin("xaxisMaterial", Ogre::RenderOperation::OT_LINE_LIST); 
    xAxis->position(-100, 0, 0); 
    xAxis->position(100, 0, 0); 
    xAxis->end(); 
    xAxisNode->attachObject(xAxis);

    Ogre::ManualObject* yAxis =  mSceneMgr->createManualObject("yaxis"); 
    Ogre::SceneNode* yAxisNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("yaxisNode"); 

    Ogre::MaterialPtr yAxisMaterial = Ogre::MaterialManager::getSingleton().create("yaxisMaterial","debugger"); 
    yAxisMaterial->setReceiveShadows(false); 
    yAxisMaterial->getTechnique(0)->setLightingEnabled(true); 
    yAxisMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,1,0,0); 
    yAxisMaterial->getTechnique(0)->getPass(0)->setAmbient(0,1,0); 
    yAxisMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,1,0); 
    //yAxisMaterial->dispose();  // dispose pointer, not the material
    yAxis->begin("yaxisMaterial", Ogre::RenderOperation::OT_LINE_LIST); 
    yAxis->position(0, -100, 0); 
    yAxis->position(0, 100, 0); 
    yAxis->end(); 
    yAxisNode->attachObject(yAxis);

    new TextRenderer(); // initialize TextRenderer singleton
    // FIXME: change on resize, depend on resolution.
    TextRenderer::getSingleton().addTextBox("XLabel", "+X", 1200, 320, 100, 20, Ogre::ColourValue::Blue);
    TextRenderer::getSingleton().addTextBox("YLabel", "+Y", 640, 10, 100, 20, Ogre::ColourValue::Green);
}

int main(int argc, char *argv[])
{
    attachInterruptHandlers();

    // Create application object
    ParticlesApplication app;

    // TODO: call Ogre::Root::addResourceLocation (const String &name, const String &locType, const String &groupName=ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, bool recursive=false)

    try 
    {
        app.go();
    } 
    catch(const Ogre::Exception& e)
    {
        std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
    }
    return 0;
}


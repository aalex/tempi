/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
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
#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <tr1/memory>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class BaseApplication : 
    public Ogre::FrameListener, 
    public Ogre::WindowEventListener, 
    public OIS::KeyListener, 
    public OIS::MouseListener
{
    public:
        BaseApplication();
        virtual ~BaseApplication();

        virtual void go();

    protected:
        virtual bool setup();
        virtual bool configure();
        virtual void chooseSceneManager();
        virtual void createCamera();
        virtual void createFrameListener();
        virtual void createScene() = 0; // Override me!
        virtual void destroyScene();
        virtual void createViewports();
        virtual void setupResources();
        virtual void createResourceListener();
        virtual void loadResources();

        // Ogre::FrameListener
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        // OIS::KeyListener
        virtual bool keyPressed( const OIS::KeyEvent &arg );
        virtual bool keyReleased( const OIS::KeyEvent &arg );
        // OIS::MouseListener
        virtual bool mouseMoved( const OIS::MouseEvent &arg );
        virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

        // Ogre::WindowEventListener
        //Adjust mouse clipping area
        virtual void windowResized(Ogre::RenderWindow* rw);
        //Unattach OIS before window shutdown (very important under Linux)
        virtual void windowClosed(Ogre::RenderWindow* rw);

        std::tr1::shared_ptr<Ogre::Root> mRoot;
        Ogre::Camera* mCamera;
        Ogre::SceneManager* mSceneMgr;
        Ogre::RenderWindow* mWindow;
        Ogre::String mResourcesCfg;
        Ogre::String mPluginsCfg;

        // OgreBites
        bool mCursorWasVisible;                    // was cursor visible before dialog appeared
        bool mShutDown;

        //OIS Input devices
        OIS::InputManager* mInputManager;
        OIS::Mouse*    mMouse;
        OIS::Keyboard* mKeyboard;
};

#endif // #ifndef __BaseApplication_h_

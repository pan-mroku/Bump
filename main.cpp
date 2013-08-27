#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreLogManager.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include "object.hpp"

int main()
{
  Ogre::Root* OgreRoot=new Ogre::Root();

  Ogre::RenderWindow* Window;

  if(OgreRoot->restoreConfig() || OgreRoot->showConfigDialog())
    Window = OgreRoot->initialise(true, "Analiza detekcji kolizji."); //koniecznie tuż pod konstruktorem root!
  else
    throw -1;

  Ogre::SceneManager* SceneMgr = OgreRoot->createSceneManager(Ogre::ST_GENERIC);
  SceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
  Ogre::Light* l = SceneMgr->createLight("MainLight");
  l->setPosition(20,-80,50);

  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Suzanne", "FileSystem");
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Ponoć niektóre API to omijają, więc warto dodać
  
  Ogre::Camera* Camera = SceneMgr->createCamera("PlayerCam");
  Camera->setPosition(Ogre::Vector3(10,-50,10));
  Camera->setOrientation(Ogre::Quaternion(1,1,0,0));
  Camera->setNearClipDistance(0.1);

  Ogre::Viewport* vp = Window->addViewport(Camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

  Ogre::Timer timer;
  
  Object o;
  
  try
    {
      while(true)
        {
          // Pump window messages for nice behaviour
          Ogre::WindowEventUtilities::messagePump();

          if(Window->isClosed())
            {
              return false;
            }
          
          timer.reset();

          // Render a frame
          if(!OgreRoot->renderOneFrame()) break;
        }
    }
  catch( Ogre::Exception& e ) 
    {
      std::cerr << "An exception has occured: " <<e.getFullDescription().c_str() << std::endl;
    }
  return 0;
}

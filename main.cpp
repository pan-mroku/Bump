#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreLogManager.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <QtWidgets/QApplication>
#include "object.hpp"
#include "mainwindow.hpp"
#include "scene.hpp"
#include <list>

int main(int argc, char* argv[])
{

  QApplication app(argc, argv);

  MainWindow view;
  view.show();

  Ogre::Root* OgreRoot=new Ogre::Root;

  Ogre::RenderWindow* Window;

  if(OgreRoot->restoreConfig() || OgreRoot->showConfigDialog())
    Window = OgreRoot->initialise(false, "GJK"); //koniecznie tuż pod konstruktorem root!
  else
    throw -1;

  Ogre::NameValuePairList misc;
  misc["parentWindowHandle"] = Ogre::StringConverter::toString(view.RenderAreaId());

  Window = OgreRoot->createRenderWindow("Main RenderWindow", view.RenderAreaWidth(), view.RenderAreaHeight(), false, &misc);

  QObject::connect(&view, &MainWindow::sizeChanged, [=](int width, int height)
                   {
                     Window->resize(width,height);
                   }
                   );

  Ogre::SceneManager* SceneMgr = OgreRoot->createSceneManager(Ogre::ST_GENERIC);
  SceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
  Ogre::Light* l = SceneMgr->createLight("MainLight");
  l->setPosition(20,-80,50);

  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Resources", "FileSystem");
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Ponoć niektóre API to omijają, więc warto dodać
  
  Ogre::Camera* Camera = SceneMgr->createCamera("PlayerCam");
  Camera->setPosition(Ogre::Vector3(0,-25,0));
  Camera->setOrientation(Ogre::Quaternion(1,1,0,0));
  Camera->setNearClipDistance(0.1);

  Ogre::Viewport* vp = Window->addViewport(Camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  Camera->setAutoAspectRatio(true);

  Ogre::Timer timer;
  
  /*  Object o("Suzanne.mesh");
  Object c("Cube.mesh");
  Object c1("Cube.mesh", Ogre::Vector3(2,0,0));*/
  //  Object t("Triangle

  boost::property_tree::basic_ptree<std::string, std::string> pt;
  boost::property_tree::ini_parser::read_ini("scenes.ini", pt);
  std::list<Scene*> Scenes;
  for(auto sceneProperties : pt)
    {
      Scene* s=new Scene(sceneProperties.second);
      //QObject::connect((const Object*)s, &Scene::ObjectsCollisionInSceneChanged, &MainWindow::SwitchCollisionInScene);
      Scenes.push_back(s);
    }
  
  try
    {
      while(true)
        {
          // Pump window messages for nice behaviour
          Ogre::WindowEventUtilities::messagePump();
          
          if(view.isHidden())
            {
              return false;
            }

          app.processEvents();

          for(Scene* s : Scenes)
            s->CheckSceneCollision();
          
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

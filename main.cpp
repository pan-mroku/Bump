#include <OgreWindowEventUtilities.h>
#include <QtWidgets/QApplication>
#include <QtCore/qexception.h>
#include "object.hpp"
#include "mainwindow.hpp"
#include "scene.hpp"
#include "ogrewidget.hpp"
#include "collisiondetector.hpp"
#include <list>

int main(int argc, char* argv[])
{
  try
    {

      QApplication app(argc, argv);

      Ogre::Root* root =new Ogre::Root;
      if(root->restoreConfig() || root->showConfigDialog())
        root->initialise(false, "GJK"); //koniecznie tuż pod konstruktorem root!
      else
        throw "Ogre initialisation error.";

      Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Resources", "FileSystem");
      Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
      
      Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Ponoć niektóre API to omijają, więc warto dodać

      MainWindow view;
      view.show();
  
      Ogre::Timer frameTimer;
      Ogre::Timer deltaTimer;
      unsigned long frameCount=0;
  
      boost::property_tree::basic_ptree<std::string, std::string> pt;
      boost::property_tree::ini_parser::read_ini("scenes.ini", pt);
      std::list<std::pair<std::string, Scene*>> Scenes;

      for(auto sceneProperties : pt)
        {
          Scene* s=new Scene(sceneProperties.second);
          s->SetInactive();
          QObject::connect(s, &Scene::ObjectsCollisionInSceneChanged, &view, &MainWindow::SwitchCollisionInScene);
          Scenes.push_back(std::make_pair(sceneProperties.first,s));
          view.AddScene(sceneProperties.first);
        }
      Scenes.begin()->second->SetActive();
  
      QObject::connect(&view, &MainWindow::sceneChanged, [=](const std::string& sceneTitle)
                       {
                         for (auto scenePair : Scenes)
                           if(scenePair.second->IsActive())
                             scenePair.second->SetInactive();
                           else if(scenePair.first==sceneTitle)
                             scenePair.second->SetActive();
                       }
                       );

      CollisionDetector collisionDetector;
      QObject::connect(&collisionDetector, &CollisionDetector::ObjectsCollisionInAlgorithmChanged, &view, &MainWindow::SwitchCollisionInAlgorithm);
  

      while(true)
        {
          // Pump window messages for nice behaviour
          Ogre::WindowEventUtilities::messagePump();
          
          if(view.isHidden())
            {
              return false;
            }

          app.processEvents();

          for(auto scenePair : Scenes)
            {
              Scene* scene=scenePair.second;
              if(scene->IsActive())
                {
                  scene->CheckSceneCollision();
                  collisionDetector.CheckCollision(scene->ObjectA, scene->ObjectB);
                  if(deltaTimer.getMilliseconds()>1000/30)
                    {
                      scene->Tick(deltaTimer.getMilliseconds()*30/1000);
                      deltaTimer.reset();
                    }
                  break;
                }
            }

          // Render a frame
          if(!root->renderOneFrame()) break;
          frameCount++;

          if(frameTimer.getMilliseconds()>1000)
            {
              view.setFPS(1000*frameCount/frameTimer.getMilliseconds());
              frameCount=0;
              frameTimer.reset();
            }
          
        }
    }
  catch( Ogre::Exception& oe ) 
    {
      std::cerr << "An exception has occured: " <<oe.getFullDescription().c_str() << std::endl;
    }
  catch( QException& qe)
    {
      std::cerr<<"Exception: "<<qe.what()<<std::endl;
    }
  catch( std::exception& e)
    {
      std::cerr<<"Exception: "<<e.what()<<std::endl;
    }
  catch(char const* c)
    {
      std::cerr<<"Exception: "<<c<<std::endl;
    }
  return 0;
}

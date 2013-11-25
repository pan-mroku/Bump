#include <OgreWindowEventUtilities.h>
#include <QtWidgets/QApplication>
#include <QtCore/qexception.h>
#include "object.hpp"
#include "mainwindow.hpp"
#include "scene.hpp"
#include "ogrewidget.hpp"
#include <list>

int main(int argc, char* argv[])
{

  QApplication app(argc, argv);

  MainWindow view;
  view.show();

  Ogre::Root* root =new Ogre::Root;
  if(root->restoreConfig() || root->showConfigDialog())
    root->initialise(false, "GJK"); //koniecznie tuż pod konstruktorem root!
  else
    throw "Ogre initialisation error.";
  
  OgreWidget ogre(view.RenderAreaPtr(), "Main window");

  QObject::connect(&view, &MainWindow::sizeChanged, &ogre, &OgreWidget::resize);

  Ogre::Timer timer;
  
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

          for(auto scenePair : Scenes)
            {
              Scene* scene=scenePair.second;
              if(scene->IsActive())
                scene->CheckSceneCollision();
            }
          
          timer.reset();

          // Render a frame
          if(!ogre.Root->renderOneFrame()) break;
        }
    }
  catch( Ogre::Exception& e ) 
    {
      std::cerr << "An exception has occured: " <<e.getFullDescription().c_str() << std::endl;
    }
  catch( QException& qe)
    {
      std::cerr<<"Exception: "<<qe.what()<<std::endl;
    }
  return 0;
}

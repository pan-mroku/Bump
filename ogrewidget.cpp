#include "ogrewidget.hpp"

OgreWidget::OgreWidget(QWidget* parent, const std::string& windowName):QWidget(parent)
{
  if(!parent)
    throw "No parent widget!";

  Root=Ogre::Root::getSingletonPtr();

  if(!Root)
    throw "Invalid Ogre::Root!";

  SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  SceneManager->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
  Ogre::Light* l = SceneManager->createLight("MainLight");
  l->setPosition(20,-80,50);

  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Resources", "FileSystem");
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
      
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);//Ponoć niektóre API to omijają, więc warto dodać

  Ogre::NameValuePairList misc;
  misc["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)parent->winId());
  Window = Root->createRenderWindow(windowName, parent->width(), parent->height(), false, &misc);
  
  Camera = SceneManager->createCamera(windowName);
  Camera->setPosition(Ogre::Vector3(0,-25,0));
  Camera->setOrientation(Ogre::Quaternion(1,1,0,0));
  Camera->setNearClipDistance(0.1);

  Ogre::Viewport* vp = Window->addViewport(Camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  Camera->setAutoAspectRatio(true);
}

OgreWidget::~OgreWidget()
{
  delete Camera;
}

void OgreWidget::resize(int width, int height)
{
  Window->resize(width,height);
}


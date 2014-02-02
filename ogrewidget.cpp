#include "ogrewidget.hpp"

OgreWidget::OgreWidget(QWidget* parent):QWidget(parent), isMousePressed(false), lastMousePosition(0,0)
{
  if(!parent)
    throw "No parent widget!";

  Root=Ogre::Root::getSingletonPtr();

  if(!Root)
    throw "Invalid Ogre::Root!";

  SceneManager = Root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  SceneManager->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
  Ogre::Light* l=SceneManager->createLight();
  l->setPosition(20,-80,50);
  
  Ogre::NameValuePairList misc;
#ifdef WIN32
  misc["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)winId());
#else
  misc["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)winId());
#endif
  Window = Root->createRenderWindow(objectName().toStdString(), parent->width(), parent->height(), false, &misc);

  Camera = SceneManager->createCamera(objectName().toStdString());
  Camera->setPosition(Ogre::Vector3(0,-25,0));
  Camera->setOrientation(Ogre::Quaternion(1,1,0,0));
  Camera->setNearClipDistance(0.1);

  Focus=SceneManager->getRootSceneNode()->createChildSceneNode();
  Focus->attachObject(Camera);

  Ogre::Viewport* vp = Window->addViewport(Camera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  Camera->setAutoAspectRatio(true);

  setFocus();
}

OgreWidget::~OgreWidget()
{
  delete Camera;
}

void OgreWidget::resizeEvent(QResizeEvent* event)
{
  Window->reposition(x(), y());
  Window->resize(width(),height());
  Window->windowMovedOrResized();
}

void OgreWidget::keyPressEvent(QKeyEvent* qKeyEvent)
{
  auto key=qKeyEvent->key(); //żeby ify były szybsze

  if(key==Qt::Key_Escape);

  if(key==Qt::Key_W)
    Camera->moveRelative(Ogre::Vector3(0,0,-1));

  if(key==Qt::Key_S)
    Camera->moveRelative(Ogre::Vector3(0,0,1));

  if(key==Qt::Key_A)
    Camera->moveRelative(Ogre::Vector3(-1,0,0));

  if(key==Qt::Key_D)
    Camera->moveRelative(Ogre::Vector3(1,0,0));

  if(key==Qt::Key_R)
    Camera->moveRelative(Ogre::Vector3(0,1,0));

  if(key==Qt::Key_F)
    Camera->moveRelative(Ogre::Vector3(0,-1,0));
}

void OgreWidget::mousePressEvent(QMouseEvent* qMouseEvent)
{
  isMousePressed=true;
  lastMousePosition=qMouseEvent->localPos();
}

void OgreWidget::mouseReleaseEvent(QMouseEvent* qMouseEvent)
{
  isMousePressed=false;
}

void OgreWidget::mouseMoveEvent(QMouseEvent* qMouseEvent)
{
  if(!isMousePressed)
    return;
  
  QPointF mouseDelta=qMouseEvent->localPos()-lastMousePosition;
  lastMousePosition=qMouseEvent->localPos();

  //Obrót po Z jest globalny, a po X lokalny, stąd trzeba pilnować kolejności
  Ogre::Quaternion zRotation(1,0, 0, -mouseDelta.x()/width());
  Ogre::Quaternion xRotation(1,-mouseDelta.y()/height(), 0, 0);

  Focus->setOrientation(zRotation*Focus->getOrientation()*xRotation);
}

void OgreWidget::focusOutEvent(QFocusEvent * qFocusEvent)
{
  setFocus();
}

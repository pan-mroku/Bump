#ifndef OGREWIDGET_HPP
#define OGREWIDGET_HPP

#include <QtWidgets/QWidget>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

/** Widget Qt opakowujacy Ogre.
*/
class OgreWidget:public QWidget
{

  Q_OBJECT

public:
  /** Konstruktor.
      @param parent Wskaźnik do widgetu wyższego w hierarchii. Potrzebny tylko dla id obszaru do renderowania.
      @param windowName Unikalna nazwa używana wewnątrz Ogre.
   */
  OgreWidget(QWidget* parent, const std::string& windowName);

  ~OgreWidget();

  Ogre::Root* Root;
  Ogre::RenderWindow* Window;
  Ogre::SceneManager* SceneManager;
  Ogre::Camera* Camera;
public slots:
  void resize(int width, int height);
};

#endif

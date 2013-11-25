#ifndef OGREWIDGET_HPP
#define OGREWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QtGui/QResizeEvent>
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

  /** Destruktor.
   */
  ~OgreWidget();

  Ogre::Root* Root;
  Ogre::RenderWindow* Window;
  Ogre::SceneManager* SceneManager;
  Ogre::Camera* Camera;

public slots:

  /** Slot zmieniajacy rozmiary okna Ogre.
      @note
      Było potrzebne, bo QT 5.1 jakoś inaczej zmienia rozmiar. QResizeEvent nie pomoże, bo w QT zmienia się rozmiar widgetu, po którym po prostu rysujemy, a nie rozmiar OgreWidget.
  */
  void resize(int width, int height);
};

#endif

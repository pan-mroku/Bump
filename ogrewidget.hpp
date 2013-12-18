#ifndef OGREWIDGET_HPP
#define OGREWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QtGui/QResizeEvent>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

/** Widget Qt opakowujący Ogre.
 */
class OgreWidget:public QWidget
{

  Q_OBJECT

public:
  /** Konstruktor.
      @param parent Wskaźnik do widgetu wyższego w hierarchii.
  */
  OgreWidget(QWidget* parent);

  /** Destruktor.
   */
  ~OgreWidget();

  /** Zmienia rozmiar okna w Ogre oraz w Qwidget.*/
  void resize(int width, int height);

  Ogre::Root* Root;
  Ogre::RenderWindow* Window;
  Ogre::SceneManager* SceneManager;
  Ogre::Camera* Camera;

protected:
  virtual void keyPressEvent(QKeyEvent* qKeyEvent);
  virtual void mousePressEvent(QMouseEvent* qMouseEvent);
  virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent);
  virtual void mouseMoveEvent(QMouseEvent* qMouseEvent);
  /** Reimplementacja metody QWidget::focusOutEvent. Dzięki temu widget ogre zawsze nasłuchuje klawiaturę, bez kompletnego jej przechwytywania.
      @remarks
      W razie potrzeby kilku widżetów ogre lepiej dodać możliwość tworzenia niezachłannych obiektów.
  */
  virtual void focusOutEvent(QFocusEvent * qFocusEvent);

  bool isMousePressed;
  QPointF lastMousePosition;

};

#endif

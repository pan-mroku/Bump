#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtGui/QResizeEvent>

namespace Ui {
  class MainWindow;
}


/** Klasa głównego okna QT.
    @remarks
    Łączy kod z gui. To tutaj zdefiniowane są metody i pola do obsługi zdarzeń w okienku QT.
*/
class MainWindow : public QMainWindow
{
  Q_OBJECT

signals:
  /** Sygnał informujący o zmianie rozmiaru okna.
      @remarks
      Potrzebny dla OgreWidget. Porównaj OgreWidget::resize.
*/
  void sizeChanged(int width, int height);
  /** Sygnał informujący, że z listy została wybrana scena.*/
  void sceneChanged(const std::string& sceneTitle);
  /** Sygnał informujący, że z listy został wybrany algorytm. */
  void algorithmChanged(const std::string& algorithmTitle);
                                         

public slots:
  /** Slot powodujący zaznaczenie, że nastąpiła kolizja zapisana w scenie.*/
  void SwitchCollisionInScene(bool SceneCollisionState);
protected slots:
  void sceneDropdownChanged(const QString& sceneQTitle);
  //void algorithmDropdownChanged(const QString& algorithmQTitle);

public:
  /** Konstruktor. */
  explicit MainWindow(QWidget *parent = 0);
  /** Destruktor. */
  ~MainWindow();

  /** Wskaźnik na widget, gdzie mamy renderować.
      @remarks
      Porównaj OgreWidget::OgreWidget.
  */
  QWidget* RenderAreaPtr();

  /** Dodaj scenę do rozwijalnej listy.*/
  void AddScene(std::string sceneTitle);

protected:
  void resizeEvent(QResizeEvent * event);

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_HPP

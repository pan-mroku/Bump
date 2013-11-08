#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtGui/QResizeEvent>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

signals:
  void sizeChanged(int width, int height);

public slots:
  void SwitchCollisionInScene(bool SceneCollisionState);

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  int RenderAreaId() const;
  int RenderAreaWidth() const;
  int RenderAreaHeight() const;

protected:
  void resizeEvent(QResizeEvent * event);

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_HPP

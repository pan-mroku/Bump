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
  void sceneChanged(const std::string& sceneTitle);
  void algorithmChanged(const std::string& algorithmTitle);
                                         

public slots:
  void SwitchCollisionInScene(bool SceneCollisionState);
protected slots:
  void sceneDropdownChanged(const QString& sceneQTitle);
  //void algorithmDropdownChanged(const QString& algorithmQTitle);

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  int RenderAreaId() const;
  int RenderAreaWidth() const;
  int RenderAreaHeight() const;

  void AddScene(std::string sceneTitle);

protected:
  void resizeEvent(QResizeEvent * event);

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_HPP

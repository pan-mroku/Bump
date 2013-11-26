#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->DropdownListScene, SIGNAL(currentIndexChanged(const QString&)), SLOT(sceneDropdownChanged(const QString&)));
 
 ui->widget->grabKeyboard();
}

MainWindow::~MainWindow()
{
  delete ui;
}

OgreWidget* MainWindow::RenderAreaPtr()
{
  return ui->widget;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
  ui->widget->resize(ui->OgrePositionHack->size().width(), ui->OgrePositionHack->size().height());
}

void MainWindow::SwitchCollisionInScene(bool SceneCollisionState)
{
  if(SceneCollisionState)
    ui->DisplayCollisionScene->display(1);
  else
    ui->DisplayCollisionScene->display(0);
}

void MainWindow::sceneDropdownChanged(const QString& sceneQTitle)
{
  emit sceneChanged(sceneQTitle.toStdString());
}

void MainWindow::AddScene(std::string sceneTitle)
{
  ui->DropdownListScene->addItem(QString(sceneTitle.c_str()));
}

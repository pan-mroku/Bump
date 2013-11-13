#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->DropdownListScene, SIGNAL(currentIndexChanged(const QString&)), SLOT(sceneDropdownChanged(const QString&)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

int MainWindow::RenderAreaId() const
{
  return ui->widget->winId();
}

int MainWindow::RenderAreaWidth() const
{
  return ui->widget->width();
}

int MainWindow::RenderAreaHeight() const
{
  return ui->widget->height();
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
  emit sizeChanged(RenderAreaWidth(), RenderAreaHeight());
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

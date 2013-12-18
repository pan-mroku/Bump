#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->DropdownListScene, SIGNAL(currentIndexChanged(const QString&)), SLOT(sceneDropdownChanged(const QString&)));

  connect(ui->DropdownListAlgorithm, SIGNAL(currentIndexChanged(const QString&)), SLOT(algorithmDropdownChanged(const QString&)));
 
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
  ui->widget->resize(ui->widget->size().width(), ui->widget->size().height());
}

void MainWindow::SwitchCollisionInScene(bool SceneCollisionState)
{
  if(SceneCollisionState)
    ui->DisplayCollisionScene->display(1);
  else
    ui->DisplayCollisionScene->display(0);
}

void MainWindow::SwitchCollisionInAlgorithm(bool AlgorithmCollisionState)
{
  if(AlgorithmCollisionState)
    ui->DisplayCollisionAlgorithm->display(1);
  else
    ui->DisplayCollisionAlgorithm->display(0);
}

void MainWindow::sceneDropdownChanged(const QString& sceneQTitle)
{
  emit sceneChanged(sceneQTitle.toStdString());
}

void MainWindow::algorithmDropdownChanged(const QString& algorithmQTitle)
{
  emit algorithmChanged(algorithmQTitle.toStdString());
}

void MainWindow::AddScene(std::string sceneTitle)
{
  ui->DropdownListScene->addItem(QString(sceneTitle.c_str()));
}

void MainWindow::AddAlgorithm(std::string algorithmTitle)
{
  ui->DropdownListAlgorithm->addItem(QString(algorithmTitle.c_str()));
}

void MainWindow::setFPS(unsigned long fps)
{
  ui->FPSLabel->setText(QString::number(fps));
}

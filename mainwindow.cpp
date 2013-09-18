#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(960, 540);
    fsv_ = new FileSystemViewer(this);
}

MainWindow::~MainWindow()
{
    delete fsv_;
}


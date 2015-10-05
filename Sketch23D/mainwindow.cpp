#include "mainwindow.h"
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

void MainWindow::on_actionCreate_Mesh_triggered()
{
    ui->widget->stat = 1;
}

void MainWindow::on_actionClear_triggered()
{
    ui->widget->clearScreen();
}

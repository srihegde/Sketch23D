#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>


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
    ui->widget->stat = 2;
    ui->widget->viewMode = false;
    ui->widget->setupProjectionTransformation(false);
}

void MainWindow::on_actionClear_triggered()
{
    ui->widget->clearScreen();
}

void MainWindow::on_actionView_Model_triggered()
{
    ui->widget->viewMode = true;
    ui->widget->setupProjectionTransformation(true);
}

void MainWindow::on_actionDraw_Silehoutte_triggered()
{
    ui->widget->stat = 1;
    ui->widget->viewMode = false;
    ui->widget->setupProjectionTransformation();
}

void MainWindow::on_actionZoom_In_triggered()
{
    ui->widget->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    ui->widget->zoomOut();
}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{
//    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
//                               initialPath,
//                               tr("%1 Files (*.%2);;All Files (*)"));
//    if (fileName.isEmpty()) {
//        return false;
//    } else {
//        return ui->widget->saveImage(fileName);
//    }
}

void MainWindow::on_actionAbout_Sketch23D_triggered()
{
    QMessageBox::about(this, tr("About Sketch23D"),
            tr("<p>The <b>Sketch23D</b> is a sketching interface for quick and easy "
               "designing of freeform 3D model automatically from 2D sketches. The "
               "sketching interface is inspired from the sample implementation of "
               "Scribble application from the Qt Samples.</p><p> Some rights "
               "reserved @Srinidhi Hegde. 2020</p>"));
}

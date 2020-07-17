#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCreate_Mesh_triggered();

    void on_actionClear_triggered();

    void on_actionView_Model_triggered();

    void on_actionDraw_Silehoutte_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_Sketch23D_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

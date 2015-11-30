/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionClear;
    QAction *actionCreate_Mesh;
    QAction *actionView_Model;
    QAction *actionDraw_Silehoutte;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    OpenGLWidget *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(657, 461);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        actionCreate_Mesh = new QAction(MainWindow);
        actionCreate_Mesh->setObjectName(QStringLiteral("actionCreate_Mesh"));
        actionView_Model = new QAction(MainWindow);
        actionView_Model->setObjectName(QStringLiteral("actionView_Model"));
        actionDraw_Silehoutte = new QAction(MainWindow);
        actionDraw_Silehoutte->setObjectName(QStringLiteral("actionDraw_Silehoutte"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QStringLiteral("actionZoom_In"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widget = new OpenGLWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setCursor(QCursor(Qt::CrossCursor));

        horizontalLayout->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 657, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuEdit->addAction(actionClear);
        menuEdit->addAction(actionDraw_Silehoutte);
        menuEdit->addAction(actionCreate_Mesh);
        menuEdit->addAction(actionView_Model);
        menuView->addAction(actionZoom_In);
        menuView->addAction(actionZoom_Out);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Sketch23D", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionClear->setText(QApplication::translate("MainWindow", "Clear", 0));
        actionClear->setShortcut(QApplication::translate("MainWindow", "Shift+Del", 0));
        actionCreate_Mesh->setText(QApplication::translate("MainWindow", "Create Mesh", 0));
        actionCreate_Mesh->setShortcut(QApplication::translate("MainWindow", "Ctrl+M", 0));
        actionView_Model->setText(QApplication::translate("MainWindow", "View Model", 0));
        actionView_Model->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", 0));
        actionDraw_Silehoutte->setText(QApplication::translate("MainWindow", "Draw Silehoutte", 0));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", 0));
        actionZoom_In->setShortcut(QApplication::translate("MainWindow", "Ctrl+Up", 0));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0));
        actionZoom_Out->setShortcut(QApplication::translate("MainWindow", "Ctrl+Down", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

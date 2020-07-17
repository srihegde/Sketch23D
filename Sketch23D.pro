#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T01:04:25
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sketch23D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    shader_utils.cpp \
    gl_utils.cpp \
    mesh_utils.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    shader_utils.h \
    gl_utils.h \
    mesh_utils.h

FORMS    += mainwindow.ui

LIBS    += -lGLU -lGLEW -lgmp -L/lib64\
            `pkg-config opencv --libs`\
            -L/opt/CGAL-5.0.2/cgal-install/lib/

QMAKE_CXXFLAGS += -frounding-math -O3 -std=c++14 -DCGAL_USE_BASIC_VIEWER\

INCLUDEPATH += /opt/CGAL-5.0.2/cgal-install/include

DISTFILES += \
    fshader.fs \
    vshader.vs \
    checkered-tex.jpg

#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T01:04:25
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sketch23D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    shader_utils.cpp \
    gl_utils.cpp \
    mesher.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    shader_utils.h \
    gl_utils.h \
    mesher.h

FORMS    += mainwindow.ui

LIBS    += -lGLU -lGLEW -lCGAL -lgmp -L/lib64

QMAKE_CXXFLAGS += -frounding-math -O3

DISTFILES += \
    fshader.fs \
    vshader.vs

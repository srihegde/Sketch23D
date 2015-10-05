#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>

class OpenGLWidget : public QGLWidget, protected QOpenGLFunctions
{
public:
    OpenGLWidget(QWidget *parent);
    ~OpenGLWidget();
    void mouseFollow();
    void mouseScale();
    void clearScreen();
    void createMesh();
    int stat;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void setupModelTransformation();
    void setupViewTransformation();
    void setupProjectionTransformation();
    void createCubeObject();

    void mouseMoveEvent(QMouseEvent *mev);
    void mousePressEvent(QMouseEvent *mev);
    void mouseReleaseEvent(QMouseEvent *mev);

private:
    GLuint program;
    GLint vVertex_attrib, vColor_attrib;
    GLint vModel_uniform, vView_uniform, vProjection_uniform;
    int screen_width, screen_height;
    float cur_pos[2];

    GLuint cube_VAO; //Vertex array object for cube

};

#endif // OPENGLWIDGET_H

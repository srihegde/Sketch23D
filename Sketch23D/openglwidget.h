/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include "shader_utils.h"
#include "gl_utils.h"

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QGLWidget>
#include <QMouseEvent>

class OpenGLWidget : public QGLWidget
{
public:
    OpenGLWidget(QWidget *parent) : QGLWidget(parent) {}
    ~OpenGLWidget();
    void clearScreen();
    int stat;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void setupModelTransformation();
    void setupViewTransformation();
    void setupProjectionTransformation();
    void createSphereObject();
    void createCubeObject();
    void setupLighting();

    void mouseMoveEvent(QMouseEvent *mev);
    void mousePressEvent(QMouseEvent *mev);
    void mouseReleaseEvent(QMouseEvent *mev);
    glm::vec3 getTrackBallVector(int x, int y);
    void onIdle();
    void createMesh();

private:
    GLuint program;
    GLint vVertex_attrib, vColor_attrib;
    GLint vModel_uniform, vView_uniform, vProjection_uniform;
    int screen_width, screen_height;

    GLuint cube_VAO; //Vertex array object for cube
};

#endif // OPENGLWIDGET_H

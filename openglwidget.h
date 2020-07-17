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
#include <QOpenGLTexture>

class OpenGLWidget : public QGLWidget
{
public:
    explicit OpenGLWidget(QWidget *parent = 0);
//    OpenGLWidget();
    ~OpenGLWidget();
    void setupModelTransformation();
    void setupViewTransformation();
    void setupProjectionTransformation(bool perspective = false);
    void clearScreen();
    void zoomIn();
    void zoomOut();
    int stat;
    bool viewMode;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *mev);
    void mousePressEvent(QMouseEvent *mev);
    void mouseReleaseEvent(QMouseEvent *mev);

    void setupLighting();
    glm::vec3 getTrackBallVector(int x, int y);
    GLfloat computeZ(GLfloat x, GLfloat y);
    void onIdle();
    void createSilehoute();
    void silehouteToMesh();

private:
    GLuint program;
    GLint vVertex_attrib, vColor_attrib, tex;
    GLint vModel_uniform, vView_uniform, vProjection_uniform;
    int screen_width, screen_height;
    float zoomDist;
    std::vector<glm::vec3> buffer;
    GLuint linestrip_vao; //Vertex array object for cube
    QOpenGLTexture *texture;
};

#endif // OPENGLWIDGET_H

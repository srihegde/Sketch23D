/*
//
*/
//************************************************************************************************************

#include "openglwidget.h"
#include "mesh_utils.h"
#include <stdlib.h>
#include <vector>

#include <QMessageBox>

//Globals
int oldX, oldY, currentX, currentY,size3D;
bool isDragging=false;

glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations


OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
    setMouseTracking(true);

    QRect parentSize = parent->geometry();
    screen_width = parentSize.width();
    screen_height = parentSize.height();

    oldX = oldY = currentX = currentY = 0;
    viewMode = false;
}

OpenGLWidget::~OpenGLWidget()
{
    glDeleteProgram(program);
}

void OpenGLWidget::clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    buffer.clear();
    update();
}

void OpenGLWidget::zoomIn()
{
    zoomDist -= 5.0f;
    viewT = glm::lookAt(glm::vec3(0.0, 0.0, zoomDist), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
    update();
}

void OpenGLWidget::zoomOut()
{
    zoomDist += 5.0f;
    viewT = glm::lookAt(glm::vec3(0.0, 0.0, zoomDist), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
    update();
}



void OpenGLWidget::initializeGL()
{
#ifndef __APPLE__
    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return;
    }
#endif
    //Enable certain OpenGL states
    glEnable(GL_DEPTH_TEST); //Enable Z-buffer
    glEnable(GL_MULTISAMPLE); //Draw smoothed polygons

    this->stat = 0;

    //Create program
    program = createProgram("../vshader.vs", "../fshader.fs");

    // Loading Texture
//    texture = new QOpenGLTexture(QImage("./checkered-tex.jpg"));
//    texture->setWrapMode(QOpenGLTexture::Repeat);
//    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//    texture->setMagnificationFilter(QOpenGLTexture::Linear);
//    texture->bind(0);
//    glBindTexture(GL_TEXTURE_2D, tex);

    //Create cube VAO
    clearScreen();

    //Setup Transformations
    setupModelTransformation();
    setupViewTransformation();
    setupProjectionTransformation();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    screen_width = w;
    screen_height = h;
    glViewport(0, 0, screen_width, screen_height);
    setupProjectionTransformation();// Redo projection matrix
}

void OpenGLWidget::paintGL()
{
    // Clear the background as white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(linestrip_vao);

    // Wait for paint data for drawing
    onIdle();

    // Draw the buffer data
    if (isDragging && !this->viewMode && this->stat == 1)
        glDrawArrays(GL_LINE_STRIP,0,buffer.size());
    else if (this->stat == 2)
    {
        glDrawArrays(GL_LINES,0,buffer.size());
        this->viewMode = true;
    }
    else
        glDrawArrays(GL_LINE_LOOP,0,buffer.size());

    update();
    glBindVertexArray(0);
}



void OpenGLWidget::setupModelTransformation()
{
    //Modelling transformations (Model -> World coordinates)
    modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

    //Pass on the modelling matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}

void OpenGLWidget::setupViewTransformation()
{
    //Viewing transformations (World -> Camera coordinates
    zoomDist = 1.0f;
    viewT = glm::lookAt(glm::vec3(0.0, 0.0, zoomDist), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    //Camera at (0, 0, zoomDist) looking down the negative Z-axis in a right handed coordinate system

    //Pass on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if(vView_uniform == -1){
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void OpenGLWidget::setupProjectionTransformation(bool perspective)
{
    glm::mat4 projectionT;
    // Enable for perspective or orthographic projection
    if (perspective)
    {
        float aspect = (float)screen_width/(float)screen_height;
        projectionT = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
    }
    else if (!perspective)
    {
        float view_height = 1.0f;
        projectionT = glm::ortho(-view_height/2.0f, view_height/2.0f, -view_height/2.0f, view_height/2.0f, 0.1f, 1000.0f);
    }

    //Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if(vProjection_uniform == -1){
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}


void OpenGLWidget::setupLighting()
{
//    float light_position[] = {100, 50, 0};

//    glUseProgram(program);
//    vLightPosition_uniform = glGetUniformLocation(program, "vLightPosition");
//    if(vLightPosition_uniform == -1)
//    {
//        fprintf(stderr, "Could not bind uniform: vLightPosition\n");
//        exit(0);
//    }
//    glUniform3f(vLightPosition_uniform, light_position[0], light_position[1], light_position[2]);
}

glm::vec3 OpenGLWidget::getTrackBallVector(int x, int y)
{
    glm::vec3 p = glm::vec3(2.0*x/screen_width - 1.0, 2.0*y/screen_height - 1.0, 0.0); //Normalize to [-1, +1]
    p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

    float mag2 = p.x*p.x + p.y*p.y;
    if(mag2 <= 1.0)
        p.z = sqrt(1.0 - mag2);
    else
        p = glm::normalize(p); //Nearest point, close to the sides of the trackball
    return p;
}


// Computing z-co-ordinate of the drawn point to align
// the drawing with the plane normal to gaze direction.
GLfloat OpenGLWidget::computeZ(GLfloat x, GLfloat y)
{
    glm::vec3 va = getTrackBallVector(oldX, oldY);
    glm::vec3 vb = getTrackBallVector(currentX, currentY);
//    float angle = acos(fmin(1.0f, glm::dot(va, vb)));
    glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
//    glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
//    glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
//    modelT = glm::rotate(modelT, angle, axis_in_object_coord);
//    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
//    printf("%f %f %f\n",camera2object[2][0], camera2object[2][1], camera2object[2][2]);

//    GLfloat z = -(axis_in_camera_coord.x*x + axis_in_camera_coord.y*y)/axis_in_camera_coord.z;
//    return z;
    return 0.0f;
}


// To create silehoute.
void OpenGLWidget::createSilehoute()
{
    glUseProgram(program);

    vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }
    vColor_attrib = glGetAttribLocation(program, "vColor");
    if(vColor_attrib == -1) {
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }
    glUniform3f(vColor_attrib, 0.0f, 0.0f, 1.0f);

    //Generate VAO object
    glGenVertexArrays(1, &linestrip_vao);
    glBindVertexArray(linestrip_vao);

    // Pass a VBO for silehoute to the renderer
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer.size()*sizeof(glm::vec3), &buffer[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// To create 3D-mesh from silehoute
void OpenGLWidget::silehouteToMesh()
{

    glUseProgram(program);

    vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }
    vColor_attrib = glGetAttribLocation(program, "vColor");
    if(vColor_attrib == -1) {
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }
    glUniform3f(vColor_attrib, 1.0f, 0.0f, 0.0f);

    // Compute 3D mesh from silehoute
     buffer = meshCreator(buffer);

    //Generate VAO object
    glGenVertexArrays(1, &linestrip_vao);
    glBindVertexArray(linestrip_vao);

    // Pass a VBO for 3D mesh to the renderer
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer.size()*sizeof(glm::vec3), &buffer[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glDisableVertexAttribArray(vVertex_attrib);
//    inPts.clear();
//    this->stat = 1;
}


void OpenGLWidget::onIdle()
{
    // For viewing
    if((currentX !=oldX || currentY != oldY ) && this->viewMode)
    {
        glm::vec3 va = getTrackBallVector(oldX, oldY);
        glm::vec3 vb = getTrackBallVector(currentX, currentY);
        float angle = acos(fmin(1.0f, glm::dot(va, vb)));
        glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
        glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
        glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
        modelT = glm::rotate(modelT, angle, axis_in_object_coord);
        glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
        oldX = currentX;
        oldY = currentY;

    }

    // For drawing
    if((currentX !=oldX || currentY != oldY ) && this->stat == 1 )
    {
        float xPos = ((float)currentX/(float)screen_width - 0.5f);
        float yPos = (0.5f - (float)currentY/(float)screen_height);
        buffer.push_back(glm::vec3(xPos, yPos, 0.0f));
        createSilehoute();

        oldX = currentX;
        oldY = currentY;
    }

    if(this->stat == 2 && !this->viewMode)
    {
        if(buffer.size() < 3)
        {
            QMessageBox msgBox;
            msgBox.setText("Unable to create mesh. Insufficient Data Points!");
            msgBox.exec();
            this->stat = 2;
        }
        else
            silehouteToMesh();
    }
}


void OpenGLWidget::mouseMoveEvent(QMouseEvent *mev)
{
    if(isDragging)
    {
        currentX = mev->x();
        currentY = mev->y();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mev)
{
    isDragging = true;
    currentX = oldX = mev->x();
    currentY = oldY = mev->y();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *mev)
{
    isDragging = false;
}

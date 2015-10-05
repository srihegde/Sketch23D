#include <stdio.h>
#include <stdlib.h>

#include "openglwidget.h"
#include "shader_utils.h"
#include "gl_utils.h"
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


std::vector<float> buffer;
bool isDragging = false;
GLuint attribute_coord2d;
GLuint vbo, uniform_color;

OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
    this->stat = 0;
    setMouseTracking(true);
}

OpenGLWidget::~OpenGLWidget()
{
    glDeleteProgram(program);
}


void OpenGLWidget::clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    update();
}



void OpenGLWidget::createMesh()
{
    buffer.push_back(this->cur_pos[0]);
    buffer.push_back(this->cur_pos[1]);


    update();

    glDisableVertexAttribArray(attribute_coord2d);
    glDeleteBuffers(1, &vbo);
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
    initializeOpenGLFunctions();
    //Enable certain OpenGL states
    glEnable(GL_DEPTH_TEST); //Enable Z-buffer
    glEnable(GL_MULTISAMPLE); //Draw smoothed polygons

    //Create program
    program = createProgram("../proj/Sketch23D/vshader.vs", "../proj/Sketch23D/fshader.fs");

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    const char* uniform_name = "color";
    uniform_color = glGetUniformLocation(program, uniform_name);

    //Create cube VAO
//    createCubeObject();
    if(this->stat == 1)
        createMesh();


    //Setup Transformations
//    setupModelTransformation();
//    setupViewTransformation();
//    setupProjectionTransformation();
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
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);

    float *buf = &buffer[0];
    long unsigned s = buffer.size();

    glBufferData(GL_ARRAY_BUFFER, s, buf, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
      attribute_coord2d, // attribute
      2,                 // number of elements per vertex, here (x,y)
      GL_FLOAT,          // the type of each element
      GL_FALSE,          // take our values as-is
      0,                 // no extra data between each position
      0                  // offset of first element
    );

    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_POINTS, 0, buffer.size()/2);
    glDeleteBuffers(1, &vbo);

//    for(int i=0;i<size;i++)
//        std::cout<< v[size]<<" ";
//    std::cout<<"\n";
}




/*************************   ANSWERS START FROM HERE   *********************************/
// (Un)Comment the part(s) to check the code(s).


// Ans 1 (a)
void OpenGLWidget::setupModelTransformation()
{
    //TODO: Add code here
    // Model Transformation Logic
//    float angle = 15.0f;
//    glm::vec3 axis_z(0,0,1.0f);
//    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), axis_z);

//    //Pass on the viewing matrix to the vertex shader
//    glUseProgram(program);
//    vModel_uniform = glGetUniformLocation(program, "vModel");
//    if(vModel_uniform == -1){
//        fprintf(stderr, "Could not bind location: vModel\n");
//        exit(0);
//    }
//    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
}

// Ans 1 (b)
/*void OpenGLWidget::setupModelTransformation()
{
    //TODO: Add code here
    // Model Transformation Logic
    glm::vec3 scale(1.0f,2.0f,1.0f);
    glm::mat4 model = glm::scale(glm::mat4(1.0f),scale);

    //Pass on the viewing matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
}*/

// Ans 1 (c)
/*void OpenGLWidget::setupModelTransformation()
{
    //TODO: Add code here
    // Model Transformation Logic
    glm::vec3 trans(10.0f,20.0f,0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), trans);

    //Pass on the viewing matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
}*/

// Ans 2
/*void OpenGLWidget::setupModelTransformation()
{
    //TODO: Add code here
    // Model Transformation Logic
    glm::vec3 trans(10.0f,20.0f,0.0f);
    float angle = 15.0f;
    glm::vec3 axis_z(0,0,1);
    glm::vec3 scale(1.0f,2.0f,1.0f);

    // ORDER <A,B,C>
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), trans) * glm::scale(glm::mat4(1.0f),scale)
//            * glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);

    // ORDER <C,B,A>
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z) * glm::scale(glm::mat4(1.0f),scale)
            * glm::translate(glm::mat4(1.0f), trans);

    //Pass on the viewing matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
}*/

/*********************   Please look at the changes in code in "vshader.vs"   ************************/

/*************************   ANSWERS ENDS HERE   *********************************/




void OpenGLWidget::setupViewTransformation()
{
    //Viewing transformations (World -> Camera coordinates
//    glm::mat4 view = glm::lookAt(glm::vec3(20.0, 20.0, 100.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
//    //Camera at (0, 0, 20) looking down the negative Z-axis in a right handed coordinate system

//    //Pass on the viewing matrix to the vertex shader
//    glUseProgram(program);
//    vView_uniform = glGetUniformLocation(program, "vView");
//    if(vView_uniform == -1){
//        fprintf(stderr, "Could not bind location: vView\n");
//        exit(0);
//    }
//    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(view));
}

void OpenGLWidget::setupProjectionTransformation()
{
//    //Projection transformation (Orthographic projection)
//    float aspect = (float)screen_width/(float)screen_height;
//    float view_height = 100.0f;
//    glm::mat4 projection = glm::ortho(-view_height*aspect/2.0f, view_height*aspect/2.0f,
//                                      -view_height/2.0f, view_height/2.0f, 0.1f, 1000.0f);

//    //Pass on the projection matrix to the vertex shader
//    glUseProgram(program);
//    vProjection_uniform = glGetUniformLocation(program, "vProjection");
//    if(vProjection_uniform == -1){
//        fprintf(stderr, "Could not bind location: vProjection\n");
//        exit(0);
//    }
//    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}

void OpenGLWidget::createCubeObject()
{
//    glUseProgram(program);

//    //Bind shader variables
//    vVertex_attrib = glGetAttribLocation(program, "vVertex");
//    if(vVertex_attrib == -1) {
//        fprintf(stderr, "Could not bind location: vVertex\n");
//        exit(0);
//    }
//    vColor_attrib = glGetAttribLocation(program, "vColor");
//    if(vColor_attrib == -1) {
//        fprintf(stderr, "Could not bind location: vColor\n");
//        exit(0);
//    }

//    //Cube data
//    GLfloat cube_vertices[] = {10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10, //Front
//                   10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10}; //Back
//    GLushort cube_indices[] = {0, 2, 3, 0, 1, 2, //Front
//                4, 7, 6, 4, 6, 5, //Back
//                5, 2, 1, 5, 6, 2, //Left
//                4, 3, 7, 4, 0, 3, //Right
//                1, 0, 4, 1, 4, 5, //Top
//                2, 7, 3, 2, 6, 7}; //Bottom
//    GLfloat cube_colors[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1}; //Unique face colors

//    //Generate VAO object
//    glGenVertexArrays(1, &cube_VAO);
//    glBindVertexArray(cube_VAO);

//    //Create VBOs for the VAO
//    //Position information (data + format)
//    int nVertices = 6*2*3; //(6 faces) * (2 triangles each) * (3 vertices each)
//    GLfloat *expanded_vertices = new GLfloat[nVertices*3];
//    for(int i=0; i<nVertices; i++) {
//        expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
//        expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
//        expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
//    }
//    GLuint vertex_VBO;
//    glGenBuffers(1, &vertex_VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
//    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(vVertex_attrib);
//    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    delete []expanded_vertices;

//    //Color - one for each face
//    GLfloat *expanded_colors = new GLfloat[nVertices*3];
//    for(int i=0; i<nVertices; i++) {
//        int color_index = i / 6;
//        expanded_colors[i*3] = cube_colors[color_index*3];
//        expanded_colors[i*3+1] = cube_colors[color_index*3+1];
//        expanded_colors[i*3+2] = cube_colors[color_index*3+2];
//    }
//    GLuint color_VBO;
//    glGenBuffers(1, &color_VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
//    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_colors, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(vColor_attrib);
//    glVertexAttribPointer(vColor_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    delete []expanded_colors;

//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *mev)
{
    if(isDragging)
    {
        this->cur_pos[0] = (float)(2*(mev->x())/(float)this->screen_width) - 1;
        this->cur_pos[1] = 1 - (float)(2*(mev->y())/(float)this->screen_width);
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mev)
{
    isDragging = true;

    this->cur_pos[0] = (float)(2*(mev->x())/(float)this->screen_width) - 1;
    this->cur_pos[1] = 1 - (float)(2*(mev->y())/(float)this->screen_width);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *mev)
{
    isDragging = false;
    update();
}

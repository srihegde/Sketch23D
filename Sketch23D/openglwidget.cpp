#include "openglwidget.h"
#include "mesher.h"
#include <unistd.h>

#define BUF 100000

//Globals
int oldX, oldY, currentX, currentY;
bool isDragging=false;
std::vector<float> buffer;


glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations

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
//    update();
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
//    initializeOpenGLFunctions();
    //Enable certain OpenGL states
    glEnable(GL_DEPTH_TEST); //Enable Z-buffer
    glEnable(GL_MULTISAMPLE); //Draw smoothed polygons

    this->stat = 0;
    buffer.clear();

    //Create program
    program = createProgram("../ImageTree/vshader.vs", "../ImageTree/fshader.fs");

    //Create cube VAO
//    createCubeObject();

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
    /* Clear the background as white */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update();

    long unsigned s = buffer.size();

    //printOpenGLError();
    glUseProgram(program);
    glBindVertexArray(cube_VAO);
    onIdle();
//    glDrawArrays(GL_TRIANGLES, 0, (s/2)/3);
//    glDrawArrays(GL_POINTS,0,(s/2));
    glDrawArrays(GL_LINE_STRIP,0,(s/2 - 1));
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
    viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    //Camera at (0, 0, 20) looking down the negative Z-axis in a right handed coordinate system

    //Pass on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if(vView_uniform == -1){
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void OpenGLWidget::setupProjectionTransformation()
{
    //Projection transformation (Orthographic projection)
    float aspect = (float)screen_width/(float)screen_height;
    float view_height = 100.0f;
    //glm::mat4 projection = glm::ortho(-view_height*aspect/2.0f, view_height*aspect/2.0f, -view_height/2.0f, view_height/2.0f, 0.1f, 1000.0f);
    glm::mat4 projectionT = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

    //Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if(vProjection_uniform == -1){
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
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

void OpenGLWidget::createMesh()
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

    float *obj_vertices = &buffer[0];
    long unsigned s = buffer.size();

    GLushort obj_indices[BUF];
    for (int i = 0, j = 0; i < s/2-1; ++i, j+=2)
    {
       obj_indices[j] = i;
       obj_indices[j+1] = i+1;
    }

    //Generate VAO object
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    //Create VBOs for the VAO
    //Position information (data + format)
    GLfloat *expanded_vertices = new GLfloat[s*3];
    for(int i=0; i<(s/2-1); i++) {
        expanded_vertices[i*3] = obj_vertices[obj_indices[i]*2];
        expanded_vertices[i*3 + 1] = obj_vertices[obj_indices[i]*2+1];
        expanded_vertices[i*3 + 2] = 0.0f;
    }
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, (s/2-1)*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_vertices;


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//    for (int i = 0; i < s; ++i) {
//        printf("%f ",obj_vertices[i]);
//    }
//    printf("\n");

//    for (int i = 0; i < 2*(s/2 - 1); ++i) {
//        printf("%d ",obj_indices[i]);
//    }
//    printf("\n");
//    printf("\n");
    update();
}

void OpenGLWidget::silehouteToMesh()
{
    std::vector<float> inPts = meshCreator(buffer, screen_width, screen_height);

    // Drawing tesselated mesh.
}


void OpenGLWidget::onIdle()
{
    if((currentX !=oldX || currentY != oldY ) && this->stat == 1)
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

    if((fabs(currentX - oldX) > 3.0 && fabs(currentY - oldY) > 3.0 ) && this->stat == 2 && isDragging)
    {
        long unsigned s = buffer.size();
        float rat = (float)screen_height/(float)screen_width;

        buffer.push_back((((float)currentX - (float)screen_width/2)/7.0f) );
        buffer.push_back((((float)screen_height/2 - (float)currentY)/7.0f)  );

//        buffer.push_back((2*((float)currentX/(float)screen_width) - 1.0) * 10.0);
//        buffer.push_back((1.0 - 2*((float)currentY/(float)screen_height)) * 10.0);

        createMesh();

        oldX = currentX;
        oldY = currentY;
        update();
    }
}


void OpenGLWidget::mouseMoveEvent(QMouseEvent *mev)
{
    if(isDragging)
    {
        currentX = mev->x();
        currentY = mev->y();
        update();
//        usleep(10000);
//        buffer.push_back((((float)currentX - (float)screen_width/2)/10.0f) );
//        buffer.push_back((((float)screen_height/2 - (float)currentY)/10.0f)  );
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mev)
{
    isDragging = true;
    currentX = oldX = mev->x();
    currentY = oldY = mev->y();
    update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *mev)
{
    isDragging = false;
    for (int i = 0; i < buffer.size(); ++i) {
        printf("%f ",buffer[i]);
    }
    printf("\n");
    printf("\n");

    silehouteToMesh();
    update();
}

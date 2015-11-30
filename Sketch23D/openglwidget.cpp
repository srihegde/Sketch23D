/*
//  THINGS LEFT TO BE DONE IN 3D-PAINTER
//
//  1. Accurate drawing.
//  2. Loop end checking and validation.
//  3. File saving and loading options.
//  4. Undo and Redo options.
//  5. Plane adjustment.
//
*/
//************************************************************************************************************

#include "openglwidget.h"
#include "mesher.h"
#include <unistd.h>

#define BUF 100000

//Globals
int oldX, oldY, currentX, currentY,size3D;
bool isDragging=false;
std::vector<float> buffer;
std::vector<float> inPts;

glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations


OpenGLWidget::OpenGLWidget()
{
    setMouseTracking(true);
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
    program = createProgram("./vshader.vs", "./fshader.fs");

    // Loading Texture
//    texture = new QOpenGLTexture(QImage("./checkered-tex.jpg"));
//    texture->setWrapMode(QOpenGLTexture::Repeat);
//    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//    texture->setMagnificationFilter(QOpenGLTexture::Linear);
//    texture->bind(0);
//    glBindTexture(GL_TEXTURE_2D, tex);

    //Create cube VAO
//    createCubeObject();
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
    /* Clear the background as white */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    long unsigned s = buffer.size(), s1 = inPts.size();

    glUseProgram(program);
    glBindVertexArray(cube_VAO);


    onIdle();

//    glDrawArrays(GL_TRIANGLES, 0, (s/2)/3);
//    glDrawArrays(GL_POINTS,0,(s/2));
//    if(this->stat == 2){
        glDrawArrays(GL_LINE_STRIP,0,(s/2 - 1));
        update();
//    }
    //else if(this->stat == 3)
//        glDrawArrays(GL_LINE_STRIP,0,size3D/3/*(s/2 - 1)*/);
//        update();


    if(this->stat == 3 || this->stat == 1)
    {
        silehouteToMesh();
    }

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
    viewT = glm::lookAt(glm::vec3(0.0, 0.0, 100.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
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

    GLfloat z = -(axis_in_camera_coord.x*x + axis_in_camera_coord.y*y)/axis_in_camera_coord.z;
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
        glm::vec3 tmp = getTrackBallVector(obj_vertices[obj_indices[i]*2], obj_vertices[obj_indices[i]*2+1]);
        expanded_vertices[i*3 + 2] = tmp.z;
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

    update();
}


// To create 2D-mesh from silehoute
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

    /*buffer.clear();
    buffer.push_back(-30);
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(30);
    buffer.push_back(30);
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(-30);*/

    inPts = meshCreator(buffer, screen_width, screen_height);

//    for (int i = 0; i < inPts.size(); ++i) {
//            inPts[i] *= 1000.0f;
////            printf("%f ", inPts[i]);
//        }
//        printf("\n");

    unsigned long s = inPts.size();
    size3D = s;
    GLfloat *triangle_vertices = &inPts[0];


//    std::cout << "vector size" << inPts.size() << std::endl;

//    glVertexAttribPointer(vVertex_attrib, 2, GL_FLOAT, GL_FALSE, 0, triangle_vertices);
//    glDrawArrays(GL_LINE_LOOP, 0, 4);

//    int k = 0;
//    for (int i = 0; i < inPts.size(); i+=2)
//    {
//        triangle_vertices[k++] = inPts[i];
//        triangle_vertices[k++] = inPts[i+1];
//        triangle_vertices[k++] = 0.0f
//    }

    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);


    glBufferData(GL_ARRAY_BUFFER, size3D*sizeof(GLfloat), triangle_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);

    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    for (int i = 0; i < size3D/3; i+=3) {
        glDrawArrays(GL_LINES, i, i+9);
    }

    // std::cout << "vector size" << triangle_vertices[i] << std::endl;
    // printf("%f\n",inPts[i]);

//    delete []expanded_vertices;
    glDisableVertexAttribArray(vVertex_attrib);
    inPts.clear();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->stat = 1;
}


void OpenGLWidget::onIdle()
{
    // For viewing
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

    // For drawing
    if((fabs(currentX - oldX) > 5.0 && fabs(currentY - oldY) > 5.0 ) && this->stat == 2 )
    {
        long unsigned s = buffer.size();
        float factor = 100.0f;

//        buffer.push_back((((float)currentX - (float)screen_width/2)/11.0f) );
//        buffer.push_back((((float)screen_height/2 - (float)currentY)/11.0f)  );

        buffer.push_back((2*((float)currentX/(float)screen_width) - 1.0)*factor);
        buffer.push_back((1.0 - 2*((float)currentY/(float)screen_height))*factor);

        createSilehoute();

        oldX = currentX;
        oldY = currentY;
//        update();

    }

//    if(this->stat == 3)
//    {
//        if(buffer.size() < 3)
//            printf("Insufficient Data Points....\n");
//        else
//            silehouteToMesh();
//    }
}


void OpenGLWidget::mouseMoveEvent(QMouseEvent *mev)
{
    if(isDragging)
    {
        currentX = mev->x();
        currentY = mev->y();
//        update();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mev)
{
    isDragging = true;
    currentX = oldX = mev->x();
    currentY = oldY = mev->y();
//    updateGL();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *mev)
{
    isDragging = false;
    updateGL();
}

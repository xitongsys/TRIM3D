#include "glwt.h"
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QMutex>
#include <QtGui>
#include <QOpenGLShaderProgram>
#include <iostream>
using namespace std;


static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 1.0);\n"
    "}\n";

GLWT::GLWT(QWidget *parent):QOpenGLWidget(parent){
    pmc=NULL;
    this->setMouseTracking(true);
}

void GLWT::cleanup(){
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
}

void GLWT::wheelEvent(QWheelEvent *event){
    if(pmc==NULL) return;
    int num = event->delta();
    double dz=(pmc->zmax - pmc->zmin)/10.0;
    if(num>0){
        transZ += dz;
    }
    else{
        transZ -= dz;
    }
    this->repaint();
    event->accept();
}

void GLWT::resetView(){
    if(pmc==NULL) return;
    angleX=0; angleY=0; angleZ=0;
    transX = 0; transY = 0;
    transZ = -(pmc->zmax - pmc->zmin)*5;

    this->repaint();
}

void GLWT::mouseMoveEvent(QMouseEvent *event){
    if(pmc==NULL) return;
    static int x=-1,y=-1;
    int nx = event->x(), ny = event->y();

    if(event->buttons()==0){
        x = nx; y = ny;
        return;
    }
    else if(event->buttons()==1){
        angleX = ny-y;
        angleY = nx-x;
    }
    else if(event->buttons()==2){
        angleZ = nx-x;
    }

    this->repaint();
    x=nx; y=ny;
    event->accept();
}

void GLWT::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_vbo.create();

    m_camera.setToIdentity();
    m_camera.translate(0, 0, -400.0);

    m_program->setUniformValue(m_lightPosLoc, QVector3D(1000,1000,1000));
    m_program->release();

}

void GLWT::setupVertexAttribs(){
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    m_vbo.release();
}

void GLWT::resizeGL(int w, int h){
    m_proj.setToIdentity();
    m_proj.perspective(60, GLfloat(w)/h, 0.01, 700.0f);
}

void GLWT::drawObj(){
    if(pmc==NULL) return;
    m_data.clear();
    int lo = pmc->objs.size();
    for(int i=0; i<lo; i++){
        Object3D *pobj = &(pmc->objs[i]);
        int lf = pobj->faces.size();
        for(int i=0; i<lf; i++){
            int v0 = pobj->faces[i].vertex[0];
            int n0 = pobj->faces[i].vnorm[0];

            for(int j=2; j<pobj->faces[i].vertex.size(); j++){
                GLfloat x,y,z;
                x = pobj->points[v0].x;
                y = pobj->points[v0].y;
                z = pobj->points[v0].z;
                m_data.push_back(x);
                m_data.push_back(y);
                m_data.push_back(z);
                if(n0>=0){
                    x = pobj->vnorms[n0].x;
                    y = pobj->vnorms[n0].y;
                    z = pobj->vnorms[n0].z;
                    m_data.push_back(x);
                    m_data.push_back(y);
                    m_data.push_back(z);
                }

                int vi = pobj->faces[i].vertex[j-1];
                int ni = pobj->faces[i].vnorm[j-1];
                x = pobj->points[vi].x;
                y = pobj->points[vi].y;
                z = pobj->points[vi].z;
                m_data.push_back(x);
                m_data.push_back(y);
                m_data.push_back(z);
                if(ni>=0){
                    x = pobj->vnorms[ni].x;
                    y = pobj->vnorms[ni].y;
                    z = pobj->vnorms[ni].z;
                    m_data.push_back(x);
                    m_data.push_back(y);
                    m_data.push_back(z);
                }

                vi = pobj->faces[i].vertex[j];
                ni = pobj->faces[i].vnorm[j];
                x = pobj->points[vi].x;
                y = pobj->points[vi].y;
                z = pobj->points[vi].z;
                m_data.push_back(x);
                m_data.push_back(y);
                m_data.push_back(z);
                if(ni>=0){
                    x = pobj->vnorms[ni].x;
                    y = pobj->vnorms[ni].y;
                    z = pobj->vnorms[ni].z;
                    m_data.push_back(x);
                    m_data.push_back(y);
                    m_data.push_back(z);
                }
            }
        }
    }
}

void GLWT::drawAtom(){
    mutexLock.lock();
     for(int i=0; i<pmc->record.size(); i++){
         for(int j=0; j<pmc->record[i].size(); j++){
            int Z = pmc->record[i][j].Z;
            glColor4f(AtomColorTable[Z].r, AtomColorTable[Z].g, AtomColorTable[Z].b, AtomColorTable[Z].a);
            double x = pmc->record[i][j].pos.x;
            double y = pmc->record[i][j].pos.y;
            double z = pmc->record[i][j].pos.z;
         }
     }
    mutexLock.unlock();
}

void GLWT::paintGL(){
    if(pmc==NULL) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //m_world.setToIdentity();

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.bind();
    drawObj();
    m_vbo.allocate(m_data.constData(), m_data.size()*sizeof(GLfloat));
    setupVertexAttribs();

    QMatrix4x4 rM; rM.setToIdentity();
    rM.rotate(angleX, 1, 0, 0);
    rM.rotate(angleY, 0, 1, 0);
    rM.rotate(angleZ, 0, 0, 1);
    m_world=rM*m_world;
    angleX=0; angleY=0; angleZ=0;

    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera*m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    //glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());
    glDrawArrays(GL_TRIANGLES, 0, m_data.size()/6);
    m_program->release();
}



















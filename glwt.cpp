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
    "attribute vec4 inCol;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "varying vec4 outCol;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   outCol = inCol;\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "varying highp vec4 outCol;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = outCol.rgb;\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, outCol.a);\n"
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
    //transZ = -(pmc->zmax - pmc->zmin)*2;
    transZ = 0;
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
    m_program->bindAttributeLocation("inCol", 2);
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
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10*sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    f->glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
    m_vbo.release();
}

void GLWT::resizeGL(int w, int h){
    m_proj.setToIdentity();
    m_proj.perspective(60, double(w)/h, 0.01, 999999999.0f);
}

void GLWT::drawAxes(){
    {
        double dx=pmc->xmax - pmc->xmin + INT_MAX;
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(1); m_data.push_back(0); m_data.push_back(0); m_data.push_back(1);
        m_data.push_back(pmc->xmax + dx); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(1); m_data.push_back(0); m_data.push_back(0); m_data.push_back(1);
    }

    {
        double dy=pmc->ymax - pmc->ymin + INT_MAX;
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(1); m_data.push_back(0); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(pmc->ymax+dy); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(1); m_data.push_back(0); m_data.push_back(1);

    }

    {
        double dz=pmc->zmax - pmc->zmin + INT_MAX;
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(pmc->zmax+dz);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(1); m_data.push_back(1);

    }
}


void GLWT::drawObj(){
    if(pmc==NULL) return;
    int lo = pmc->objs.size();
    for(int i=0; i<lo; i++){
        Object3D *pobj = &(pmc->objs[i]);
        int lf = pobj->faces.size();
        for(int i=0; i<lf; i++){
            int v0 = pobj->faces[i].vertex[0];
            int n0 = pobj->faces[i].vnorm[0];

            for(int j=2; j<(int)pobj->faces[i].vertex.size(); j++){
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
                m_data.push_back(objColor.r);
                m_data.push_back(objColor.g);
                m_data.push_back(objColor.b);
                m_data.push_back(objColor.a);

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
                m_data.push_back(objColor.r);
                m_data.push_back(objColor.g);
                m_data.push_back(objColor.b);
                m_data.push_back(objColor.a);


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
                m_data.push_back(objColor.r);
                m_data.push_back(objColor.g);
                m_data.push_back(objColor.b);
                m_data.push_back(objColor.a);

            }
        }
    }
}

int GLWT::drawAtom(){
    mutexLock.lock();
    int num=0;
    vector<double> mem;
     for(int i=0; i<(int)pmc->record.size(); i++){
         for(int j=0; j<(int)pmc->record[i].size(); j++){
             for(int p=0; p<pres.size(); p++){
                 if(pres[p].check(pmc->record[i][j])){
                     int Z = pmc->record[i][j].Z;

                     drawSphere(mem, pmc->record[i][j].pos, pres[p].col, pres[p].R, pres[p].slice);
                     for(int k=0; k<mem.size(); k++){
                         m_data.push_back(mem[k]);
                     }


                     num++;

                 }

             }

         }
     }
    mutexLock.unlock();
    return num;
}

void GLWT::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);

    if(pmc==NULL) return;

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.bind();
    m_data.clear();
    drawAxes();
    drawAtom();
    drawObj();
    m_vbo.allocate(m_data.constData(), m_data.size()*sizeof(GLfloat));
    setupVertexAttribs();
    m_vbo.release();

    double x = -(pmc->xmax + pmc->xmin)/2;
    double y = -(pmc->ymax + pmc->ymin)/2;
    double z = -(pmc->zmax + pmc->zmin)/2;

    QMatrix4x4 transM; transM.setToIdentity();
    transM.translate(x,y,z);

    z = -(pmc->zmax-pmc->zmin) + transZ;
    m_camera.setToIdentity();
    m_camera.translate(0,0,z);

    QMatrix4x4 rM; rM.setToIdentity();
    rM.rotate(angleX, 1, 0, 0);
    rM.rotate(angleY, 0, 1, 0);
    rM.rotate(angleZ, 0, 0, 1);
    m_world=rM*m_world;
    angleX=0; angleY=0; angleZ=0;


    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera*m_world*transM);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    //glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());
    //glDrawArrays(GL_POINTS, 0, atomNum);
    //glDrawArrays(GL_TRIANGLES, atomNum*10, (m_data.size()-atomNum*10)/10);
    glDrawArrays(GL_LINES, 0, 6);
    glDrawArrays(GL_TRIANGLES, 6, (m_data.size()/10) - 6);
    m_program->release();
}




















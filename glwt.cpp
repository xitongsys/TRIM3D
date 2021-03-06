#include "glwt.h"
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QMutex>
#include <QtGui>
#include <QOpenGLShaderProgram>
#include <iostream>
#include "datainfo.h"


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
    this->setMouseTracking(true);
    pp=(MainWindow*)parent;
}

void GLWT::cleanup(){
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
}

void GLWT::wheelEvent(QWheelEvent *event){
    if(cd.pmc==NULL) return;
    int num = event->delta();

    if(cd.drawInfo.projType==1){
        double dz=(cd.pmc->zmax - cd.pmc->zmin)/10.0;
        if(num>0){
            cd.drawInfo.transZ += dz;
        }
        else{
            cd.drawInfo.transZ -= dz;
        }
    }
    else if(cd.drawInfo.projType==-1){
        double dx=(cd.pmc->xmax - cd.pmc->xmin)/10.0;
        if(num>0){
            cd.drawInfo.orthDX -= dx;
        }
        else{
            cd.drawInfo.orthDX += dx;
        }
    }

    this->repaint();
    event->accept();
}

void GLWT::resetView(){
    if(cd.pmc==NULL) return;
    cd.drawInfo.angleX=0; cd.drawInfo.angleY=0; cd.drawInfo.angleZ=0;
    cd.drawInfo.transX = 0; cd.drawInfo.transY = 0;
    //transZ = -(pmc->zmax - pmc->zmin)*2;
    cd.drawInfo.transZ = 0;
}

void GLWT::mouseMoveEvent(QMouseEvent *event){
    if(cd.pmc==NULL) return;
    static int x=-1,y=-1;
    int nx = event->x(), ny = event->y();

    if(event->buttons()==0){
        x = nx; y = ny;
        return;
    }
    else if(event->buttons()==1){
        cd.drawInfo.angleX = ny-y;
        cd.drawInfo.angleY = nx-x;
    }
    else if(event->buttons()==2){
        cd.drawInfo.angleZ = nx-x;
    }

    this->repaint();
    x=nx; y=ny;
    event->accept();
}

void GLWT::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(cd.drawInfo.bgColor.r, cd.drawInfo.bgColor.g, cd.drawInfo.bgColor.b, cd.drawInfo.bgColor.a);

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
    if(cd.pmc==NULL)return;
    setProj(w,h);

}


void GLWT::setProj(int w, int h){

    m_proj.setToIdentity();

    if(cd.drawInfo.projType==1){
        m_proj.perspective(60, double(w)/h, 0.01, 999999999.0f);
    }
    else{
        double xL=cd.pmc->xmax - cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin;
        xL=3*xL + 2*cd.drawInfo.orthDX;
        xL=max(xL,0.0);
        yL=(double)h/w*xL;
        m_proj.ortho(-xL/2, xL/2, - yL/2, yL/2, -9999999.0f, 9999999.0f);
    }
}


long GLWT::drawSelectBox(){
    long pnum=0;
    vector<double> mem;
    double xL=cd.pmc->xmax-cd.pmc->xmin, yL=cd.pmc->ymax-cd.pmc->ymin, zL=cd.pmc->zmax-cd.pmc->zmin;
    double R=2*sqrt(xL*xL + yL*yL + zL*zL);
    double L=R/2;
    double cx=(cd.pmc->xmax+cd.pmc->xmin)/2, cy=(cd.pmc->ymax+cd.pmc->ymin)/2, cz=(cd.pmc->zmax+cd.pmc->zmin)/2;
    Vect cp(cx,cy,cz);
    Vect dir=cd.plotInfo.plotDir;

    Vect vo=dir*(-L); vo=vo + cp;

    Vect posL=dir*cd.plotInfo.posL, posR=dir*cd.plotInfo.posR;
    posL = vo + posL; posR = vo + posR;

    double Ry=0,Rz=0;
    Vect tmp=posR-posL;
    if(tmp*dir<0){
        dir=dir*-1.0;
    }
    dir.getAngle(Rz, Ry);

    double H=posL.dis(posR);
    Color4f color(1,0,0,0.5);
    drawCylinder(mem, posL, color, R, H, 20, Ry, Rz);
    pnum = mem.size()/10;
    for(int i=0; i<mem.size(); i++){
        m_data.push_back(mem[i]);
    }
    return pnum;

}

long GLWT::drawAxes(){
    long pnum=0;
    {
        double dx=cd.pmc->xmax - cd.pmc->xmin + INT_MAX;
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(1); m_data.push_back(0); m_data.push_back(0); m_data.push_back(1);
        m_data.push_back(cd.pmc->xmax + dx); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(1); m_data.push_back(0); m_data.push_back(0); m_data.push_back(1);
    }

    {
        double dy=cd.pmc->ymax - cd.pmc->ymin + INT_MAX;
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(1); m_data.push_back(0); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(cd.pmc->ymax+dy); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(1); m_data.push_back(0); m_data.push_back(1);

    }

    {
        double dz=cd.pmc->zmax - cd.pmc->zmin + INT_MAX;
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(0);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(cd.pmc->zmax+dz);
        m_data.push_back(1); m_data.push_back(1); m_data.push_back(1);
        m_data.push_back(0); m_data.push_back(0); m_data.push_back(1); m_data.push_back(1);

    }
    pnum+=6;

    vector<double> mem;
    if(cd.drawInfo.projType==1){
        drawAxes3D(mem,2);
    }
    else{
        int w,h;
        w=this->width(); h=this->height();
        double xL=cd.pmc->xmax - cd.pmc->xmin;
        xL=3*xL + 2*cd.drawInfo.orthDX;
        double yL=(double)h/w*xL;

        drawAxes3D(mem, min(xL,yL)/30.0);
    }
    pnum+=mem.size()/10;
    for(int i=0; i<mem.size(); i++){
        m_data.push_back(mem[i]);
    }

    return pnum;


}

long GLWT::drawObj(){
    long pnum=0;
    if(cd.pmc==NULL) return 0;
    int lo = cd.pmc->objs.size();
    for(int i=0; i<lo; i++){
        for(int p=0; p<(int)cd.drawInfo.pres.size(); p++){
            if(!cd.drawInfo.pres[p].checkObj(i))continue;

            Object3D *pobj = &(cd.pmc->objs[i]);
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
                    m_data.push_back(cd.drawInfo.pres[p].col.r);
                    m_data.push_back(cd.drawInfo.pres[p].col.g);
                    m_data.push_back(cd.drawInfo.pres[p].col.b);
                    m_data.push_back(cd.drawInfo.pres[p].col.a);

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
                    m_data.push_back(cd.drawInfo.pres[p].col.r);
                    m_data.push_back(cd.drawInfo.pres[p].col.g);
                    m_data.push_back(cd.drawInfo.pres[p].col.b);
                    m_data.push_back(cd.drawInfo.pres[p].col.a);


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
                    m_data.push_back(cd.drawInfo.pres[p].col.r);
                    m_data.push_back(cd.drawInfo.pres[p].col.g);
                    m_data.push_back(cd.drawInfo.pres[p].col.b);
                    m_data.push_back(cd.drawInfo.pres[p].col.a);

                    pnum+=3;

                }
            }



        }
    }

    return pnum;
}

long GLWT::drawAtom(){
    long pnum=0;
    mutexLock.lock();
    vector<double> mem;
     for(int i=0; i<(int)cd.pmc->record.size(); i++){
         for(int j=0; j<(int)cd.pmc->record[i].size(); j++){
             for(int p=0; p<cd.drawInfo.pres.size(); p++){
                 if(cd.drawInfo.pres[p].check(cd.pmc->record[i][j])){
                     int Z = cd.pmc->record[i][j].Z;
                     mem.clear();
                     drawSphere(mem, cd.pmc->record[i][j].pos, cd.drawInfo.pres[p].col, cd.drawInfo.pres[p].R, cd.drawInfo.pres[p].slice);
                     //drawCylinder(mem, pmc->record[i][j].pos, pres[p].col, pres[p].R, pres[p].R, pres[p].slice, CPI/4, CPI/4);
                     //drawAxes3D(mem);
                     for(int k=0; k<mem.size(); k++){
                         m_data.push_back(mem[k]);
                     }
                     pnum+=mem.size()/10;

                 }

             }

         }
     }
    mutexLock.unlock();
    return pnum;
}

void GLWT::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(cd.drawInfo.bgColor.r, cd.drawInfo.bgColor.g, cd.drawInfo.bgColor.b, cd.drawInfo.bgColor.a);

    if(cd.pmc==NULL) return;

    setProj(this->width(), this->height());

    double x = -(cd.pmc->xmax + cd.pmc->xmin)/2;
    double y = -(cd.pmc->ymax + cd.pmc->ymin)/2;
    double z = -(cd.pmc->zmax + cd.pmc->zmin)/2;

    QMatrix4x4 transM; transM.setToIdentity();
    transM.translate(x,y,z);

    z = -(cd.pmc->zmax-cd.pmc->zmin) + cd.drawInfo.transZ;
    m_camera.setToIdentity();
    m_camera.translate(0,0,z);

    QMatrix4x4 rM; rM.setToIdentity();
    rM.rotate(cd.drawInfo.angleX, 1, 0, 0);
    rM.rotate(cd.drawInfo.angleY, 0, 1, 0);
    rM.rotate(cd.drawInfo.angleZ, 0, 0, 1);
    m_world=rM*m_world;

    QMatrix4x4 mr=m_world.inverted();
    cd.plotInfo.plotDir.x = mr.row(0).x();
    cd.plotInfo.plotDir.y = mr.row(1).x();
    cd.plotInfo.plotDir.z = mr.row(2).x();

    cd.drawInfo.angleX=0; cd.drawInfo.angleY=0; cd.drawInfo.angleZ=0;

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.bind();
    m_data.clear();
    long axesPNum=0, atomPNum=0, objPNum=0;
    long selectBoxPNum=0;

    axesPNum = drawAxes();
    if(cd.drawInfo.ifShow==1){
        atomPNum = drawAtom();
    }
    objPNum = drawObj();

    if(cd.drawInfo.ifDrawSelectBox==1){
        selectBoxPNum = drawSelectBox();
    }

    m_vbo.allocate(m_data.constData(), m_data.size()*sizeof(GLfloat));
    setupVertexAttribs();
    m_vbo.release();




    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);

    m_program->setUniformValue(m_mvMatrixLoc, m_camera*m_world*transM);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    if(cd.drawInfo.ifDrawAxesLine==1){
        glDrawArrays(GL_LINES, 0, 6);
    }

    glDrawArrays(GL_TRIANGLES, axesPNum, (m_data.size()/10) - axesPNum);

    int w=this->width(), h=this->height();

    if(cd.drawInfo.projType==1){
        double xL=20*7, yL=(double)h/w*xL;
        double zL=yL/2/tan(CPI/6);
        transM.setToIdentity();
        transM.translate(-xL/2+10, -yL/2+10, -zL);
        m_program->setUniformValue(m_mvMatrixLoc, transM*m_world);
    }
    else{
        transM.setToIdentity();
        double xL=cd.pmc->xmax-cd.pmc->xmin;
        xL=xL*3 + 2*cd.drawInfo.orthDX;
        double yL=(double)h/w*xL;
        transM.translate(-xL/2 + xL/10, -yL/2 + yL/10, 0);
        m_program->setUniformValue(m_mvMatrixLoc, transM*m_world);
    }

    if(cd.drawInfo.ifDrawAxes3D==1){
        glDrawArrays(GL_TRIANGLES, 6, axesPNum-6);
    }
    m_program->release();
}




















#include <GL/glut.h>
#include "glwt.h"
#include <QOpenGLFunctions>
#include <QMouseEvent>

GLWT::GLWT(QWidget *parent):QOpenGLWidget(parent){
    pmc=NULL;
    this->setMouseTracking(true);
}

void GLWT::wheelEvent(QWheelEvent *event){
    int num = event->delta()/8;
    transZ += num;
    this->repaint();
    event->accept();
}

void GLWT::resetView(){
    angleX=0; angleY=0; angleZ=0;
    transX = 0; transY = 0;
    transZ = -200;

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
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
}

void GLWT::resizeGL(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 100, -100);
}

void GLWT::drawObj(){
    if(pmc==NULL) return;
    glColor4f(objColor.r, objColor.g, objColor.b, objColor.a);
    int lo = pmc->objs.size();
    for(int i=0; i<lo; i++){
        Object3D *pobj = &(pmc->objs[i]);
        int lf = pobj->faces.size();
        for(int i=0; i<lf; i++){
            glBegin(GL_POLYGON);
            for(int j=0; j<pobj->faces[i].vertex.size(); j++){
                GLfloat x,y,z;
                int vi = pobj->faces[i].vertex[j];
                int ni = pobj->faces[i].vnorm[j];
                if(ni>=0){
                    x = pobj->vnorms[ni].x;
                    y = pobj->vnorms[ni].y;
                    z = pobj->vnorms[ni].z;
                    glNormal3f(x,y,z);
                }
                x = pobj->points[vi].x;
                y = pobj->points[vi].y;
                z = pobj->points[vi].z;
                glVertex3f(x,y,z);
            }
            glEnd();
        }
    }
}

void GLWT::drawAtom(){
     for(int i=0; i<pmc->record.size(); i++){
         for(int j=0; j<pmc->record[i].size(); j++){
            int Z = pmc->record[i][j].Z;
            glColor4f(AtomColorTable[Z].r, AtomColorTable[Z].g, AtomColorTable[Z].b, AtomColorTable[Z].a);
            double x = pmc->record[i][j].pos.x;
            double y = pmc->record[i][j].pos.y;
            double z = pmc->record[i][j].pos.z;
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(x,y,z);
            gluSphere(gluNewQuadric(), atomSize, atomSlice, atomSlice);
            glPopMatrix();
         }
     }
}

void GLWT::paintGL(){
    if(pmc==NULL) return;

    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    GLfloat ambientLight[]={0.2f,0.2f,0.2f,1.0f};//白色主光源环境光
    GLfloat diffuseLight[]={0.8f,0.8f,0.8f,1.0f};//白色主光源漫反射
    GLfloat specularLight[]={1.0f,1.0f,1.0f,1.0f};//白色主光源镜面光
    GLfloat lightPos[] = {1, 5, 5, 1};

    glShadeModel ( GL_SMOOTH );

    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(angleX, 1, 0, 0); glRotatef(angleY, 0, 1, 0); glRotatef(angleZ, 0, 0, 1);
    angleX=0; angleY=0; angleZ=0;
    glMultMatrixf(MR);
    glGetFloatv(GL_MODELVIEW_MATRIX, MR);

    glLoadIdentity();

    double x=-(pmc->xmax+pmc->xmin)/2;
    double y=-(pmc->ymax+pmc->ymin)/2;
    double z=-(pmc->zmax+pmc->zmin)/2;

    glTranslatef(transX,transY,transZ);
    glMultMatrixf(MR);
    glTranslatef(x,y,z);

    glEnable(GL_COLOR_MATERIAL);                        //启用材质的颜色跟踪
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);   //指定材料着色的面
    glMaterialfv(GL_FRONT,GL_SPECULAR,specularLight);   //指定材料对镜面光的反应
    glMateriali(GL_FRONT,GL_SHININESS,100);             //指定反射系数
    drawAtom(); drawObj();

    glLoadIdentity();

}

/*
#include "mainwindow.h"
#include <QApplication>
#include "sample.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Sample sample;
    sample.loadInput("input.txt");

    return a.exec();
}
*/


#include <GL/glut.h>
#include <global.h>
#include "sample.h"

GLfloat x,y;
GLfloat dx,dy;
GLfloat wH,wW;
GLfloat angle=0;
GLfloat angle2=0;

Sample sample;

void drawObj(Object3D & obj){
    int lf = obj.faces.size();
    for(int i=0; i<lf; i++){
        glBegin(GL_POLYGON);
        for(int j=0; j<obj.faces[i].vertex.size(); j++){
            GLfloat x,y,z;
            int vi = obj.faces[i].vertex[j];
            int ni = obj.faces[i].vnorm[j];
            if(ni>=0){
                x = obj.vnorms[ni].x;
                y = obj.vnorms[ni].y;
                z = obj.vnorms[ni].z;
                glNormal3f(x,y,z);
                //cout<<x<<" "<<y<<" "<<z<<endl;
                //cout<<ni<<endl;
            }
            x = obj.points[vi].x;
            y = obj.points[vi].y;
            z = obj.points[vi].z;
            glVertex3f(x,y,z);
        }
        glEnd();
    }
}


void onSizeChange(GLsizei w, GLsizei h){
    glViewport(0,0,w,h);
    wH=h; wW=w;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, sample.zmax, sample.zmin);
    //glOrtho(sample.xmin, sample.xmax, sample.ymin, sample.ymax, sample.zmax, sample.zmin);

}

void renderScene(void){
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    GLfloat ambientLight[]={0.2f,0.2f,0.2f,1.0f};//白色主光源环境光
    GLfloat diffuseLight[]={0.8f,0.8f,0.8f,1.0f};//白色主光源漫反射
    GLfloat specularLight[]={1.0f,1.0f,1.0f,1.0f};//白色主光源镜面光
    GLfloat lightPos[] = {1, 5, 5, 1};

    //glShadeModel ( GL_SMOOTH );

    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(2,2,sample.zmax+20, 0,0,0, 0,1,0);

    glRotatef(angle/3.14*180, 1, 0, 0);
    glRotatef(angle2/3.14*180, 0, 1, 0);

    glEnable(GL_COLOR_MATERIAL);                        //启用材质的颜色跟踪
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);   //指定材料着色的面
    glMaterialfv(GL_FRONT,GL_SPECULAR,specularLight);   //指定材料对镜面光的反应
    glMateriali(GL_FRONT,GL_SHININESS,100);             //指定反射系数

    glColor3f(0.1, 1, 0);

    for(int i=0; i<sample.objs.size();i++){
        drawObj(sample.objs[i]);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutSwapBuffers();
}

void onKey(int key, int x, int y){
    if(key == GLUT_KEY_UP){
        angle-=0.1;
    }
    else if(key == GLUT_KEY_DOWN){
        angle+=0.1;
    }
    else if(key == GLUT_KEY_LEFT){
        angle2-=0.1;
    }
    else if(key == GLUT_KEY_RIGHT){
        angle2+=0.1;
    }


    glutPostRedisplay();
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Simple");
    glutDisplayFunc(renderScene);
    glutReshapeFunc(onSizeChange);
    glutSpecialFunc(onKey);

    sample.loadInput("/home/zxt/input.txt");
    glutMainLoop();

    return 0;
}

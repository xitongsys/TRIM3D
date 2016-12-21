#include "mainwindow.h"
#include <QApplication>
#include "sample.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


/*


#include <GL/glut.h>
#include <global.h>
#include <pthread.h>
#include "mc.h"

GLfloat x,y;
GLfloat dx,dy;
GLfloat wH,wW;
GLfloat angle=0;
GLfloat angle2=0;
GLfloat eyeZ=0;

MC mc("/home/zxt/t.t3d", "/home/zxt/SCOEF.88");

void *run(void *arg){
    mc.run();
    return (void*)NULL;
}

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


void drawAtom(){
    glColor4f(1,0,0,0.7);
    for(int i=0; i<mc.record.size(); i++){
        for(int j=0; j<mc.record[i].size(); j++){
            double x = mc.record[i][j].pos.x;
            double y = mc.record[i][j].pos.y;
            double z = mc.record[i][j].pos.z;
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(x,y,z);
            //glutSolidSphere(2, 10,10);
            gluSphere(gluNewQuadric(), 2, 10, 10);

            glPopMatrix();

        }
    }
}



void onSizeChange(GLsizei w, GLsizei h){
    glViewport(0,0,w,h);
    wH=h; wW=w;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, mc.zmax+50, mc.zmin-50);
    //glOrtho(mc.xmin, mc.xmax, mc.ymin, mc.ymax, mc.zmax, mc.zmin);

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

    gluLookAt((mc.xmax+mc.xmin)/2,(mc.ymax+mc.ymin)/2,mc.zmax+(mc.zmax-mc.zmin)+eyeZ, 0,0,0, 0,1,0);

    glRotatef(angle/3.14*180, 1, 0, 0);
    glRotatef(angle2/3.14*180, 0, 1, 0);

    glEnable(GL_COLOR_MATERIAL);                        //启用材质的颜色跟踪
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);   //指定材料着色的面
    glMaterialfv(GL_FRONT,GL_SPECULAR,specularLight);   //指定材料对镜面光的反应
    glMateriali(GL_FRONT,GL_SHININESS,100);             //指定反射系数

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //指定混合函数
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);

    drawAtom();

    glColor4f(0.5, 0.5, 0.5, 0.5);

    for(int i=0; i<mc.objs.size();i++){
        drawObj(mc.objs[i]);
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

void onMouse(int button, int state, int x, int y){
    if(button==4){
        eyeZ += 10;
    }
    else if(button==3){
        eyeZ -= 10;
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
    glutMouseFunc(onMouse);

    pthread_t tid;
    //pthread_create(&tid,NULL,run,NULL);
    mc.run();

    glutMainLoop();
    return 0;
}



*/

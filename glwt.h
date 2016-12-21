#ifndef GLWT_H
#define GLWT_H

#include <QOpenGLWidget>
#include "drawinfo.h"
#include "mc.h"


class GLWT : public QOpenGLWidget, DrawInfo {

public:
    MC *pmc;

public:
    GLWT(QWidget *parent);

    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resetView();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void drawObj();
    void drawAtom();
};

#endif // GLWT_H

#ifndef GLWT_H
#define GLWT_H

#include <QOpenGLWidget>
#include "drawinfo.h"
#include "mc.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix>
#include <QOpenGLFunctions>
#include <QVector>
#include <QOpenGLShaderProgram>
#include "object3d.h"
#include "geometry.h"


class GLWT : public QOpenGLWidget, public DrawInfo, public Geometry, public QOpenGLFunctions{

public:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_lightPosLoc;
    int m_normalMatrixLoc;
    QMatrix4x4 m_proj, m_camera, m_world;
    QVector<GLfloat> m_data;

public:
    MC *pmc;
    int ifshow;

public:
    GLWT(QWidget *parent);

    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resetView();
    void cleanup();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    long drawObj();
    long drawAxes();
    long drawAtom();
    void setupVertexAttribs();
};

#endif // GLWT_H

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
#include "logo.h"


class GLWT : public QOpenGLWidget, DrawInfo, QOpenGLFunctions{

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

    Logo m_logo;

public:
    MC *pmc;

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
    void drawObj();
    void drawAtom();
    void setupVertexAttribs();
};

#endif // GLWT_H

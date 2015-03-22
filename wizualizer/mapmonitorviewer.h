#ifndef MAPMONITORVIEWER_H
#define MAPMONITORVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QMatrix4x4>

class QOpenGLShaderProgram;
class QMatrix4x4;

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class MapMonitorViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    MapMonitorViewer(QWidget * parent = 0);
    ~MapMonitorViewer();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    void drawTriangle();
    void cleanup();
    void setupVertexAttribs();

    QOpenGLWidget * _widget;
    QOpenGLContext * _context;
    QOpenGLShaderProgram * _program;

    GLuint _attribPos;
    GLuint _attribCol;
    GLuint _uniformMatrix;
    GLuint m_frame;
    QMatrix4x4 m_projection;




    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    bool m_transparent;
};

#endif // MAPMONITORVIEWER_H

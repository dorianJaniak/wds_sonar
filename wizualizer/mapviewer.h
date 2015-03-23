#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QObject>
#include <QWidget>
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>


class MapViewer : public QOpenGLWidget, public QOpenGLFunctions
{
public:
    MapViewer(QWidget *parent = 0);
    ~MapViewer();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    void clean();
    QOpenGLShaderProgram * _program;
    QMatrix4x4 _projMat;
    GLuint _projMatID;

    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;

};

#endif // MAPVIEWER_H

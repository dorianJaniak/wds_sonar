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

class EnvMap;

class MapViewer : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
    QObject * m_parent;
public:
    MapViewer(QWidget *parent = 0);
    ~MapViewer();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    void addEnvMap(QVector<QVector<QVector4D> *> *verts, QVector4D center);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;

private:
    void addTestTriangle();
    void addGrid(float space = 1.0f, int rows = 11, int cols = 11);


    void clean();

    QOpenGLFunctions * f;

    //Shadery
    QOpenGLShaderProgram * _program;
    GLuint _materialColorID;

    //Ustawienie widoku
    QMatrix4x4 _projMat;
    QMatrix4x4 _centerMoveMat;
    GLuint _projMatID;
    GLuint _centerMoveMatID;
    GLfloat _cameraAngleX;
    GLfloat _cameraAngleY;
    GLfloat _cameraFar;
    QPoint _mouseLastPos;

    //Przykladowy obiekt
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;

    //Siatka - obiekt
    QOpenGLVertexArrayObject _gridVAO;
    QOpenGLBuffer _gridVBO;
    unsigned int _gridCountOfVerts;
    GLfloat * _gridVertices;

    //Mapy - obiekty
    QVector<EnvMap*> m_maps;
    QVector<QOpenGLVertexArrayObject*> m_mapsVAOs;
    QVector<QVector<QOpenGLBuffer*>> m_mapsVBOs;
    //New
    //QOpenGLShaderProgram * _mapProgram;
    //QOpenGLShaderProgram * _gridProgram;
    //QOpenGLShaderProgram * _robotProgram;
    //QVector<QOpenGLBuffer> _mapsVBO;      //Na jedno VAO przypadnie więcej niż jedno VBO, ponieważ siatka może być rozerwana (gdy duża odległość)

    //QOpenGLBuffer _robotVBO;
    //QVector<QOpenGLVertexArrayObject> _mapsVAO;

    //QOpenGLVertexArrayObject _robotVAO;

    //Do poprawy:


};

#endif // MAPVIEWER_H

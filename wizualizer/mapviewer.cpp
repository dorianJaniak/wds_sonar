#include "mapviewer.h"

#include <QOpenGLContext>
#include <QDebug>
#include <QMouseEvent>

const GLfloat vertices [] = {
    -0.6f, 0.0f, 0.0f, 1.0f,      //V1
    0.0f, -0.2f, 0.0f, 1.0f,     //V2
    0.0f, 0.0f, 0.0f, 1.0f,       //V3
    -0.3f, -0.4f, 0.0f, 1.0f
};


MapViewer::MapViewer(QWidget * parent) :
    QOpenGLWidget(parent),
    _vao(parent),
    _gridVAO(parent)
{
    _cameraAngleX = 0.0f;
    _cameraAngleY = 0.0f;
    _cameraFar = -1.0f;
}

MapViewer::~MapViewer()
{

}

QSize MapViewer::minimumSizeHint() const
{
    return QSize(300,200);
}

QSize MapViewer::sizeHint() const
{
    return QSize(450,500);
}

void MapViewer::initializeGL()
{
    f = QOpenGLContext::currentContext()->functions();
    initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MapViewer::clean);



    glClearColor(0.0f,0.1f,0.4f,1.0f);

    _program = new QOpenGLShaderProgram;
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/colorS.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/colorS.fs");
    _program->bindAttributeLocation("vVertex",0);
    _program->link();
    _program->bind();

    _projMatID = _program->uniformLocation("Mproj");
    _centerMoveMatID = _program->uniformLocation("Mmove");
    _materialColorID = _program->uniformLocation("materialColor");

    addTestTriangle();
    addGrid();
    _projMat.setToIdentity();
    _centerMoveMat.setToIdentity();
    _program->release();
}

void MapViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    _centerMoveMat.setToIdentity();
    _centerMoveMat.translate(0.0f,-0.2f,_cameraFar);
   // _centerMoveMat.rotate(kat, 0.0f, 1.0f, 0.0f);
    _centerMoveMat.rotate(_cameraAngleX, 1.0f,0.0f,0.0f);
    _centerMoveMat.rotate(_cameraAngleY, 0.0f,1.0f,0.0f);


    _vao.bind();
    _program->bind();
    _program->setUniformValue(_projMatID, _projMat);
    _program->setUniformValue(_centerMoveMatID, _centerMoveMat);
    _program->setUniformValue(_materialColorID, QVector4D(0.0f,1.0f,0.0f,1.0f));

    f->glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    _gridVAO.bind();
    _program->bind();
    _program->setUniformValue(_materialColorID, QVector4D(1.0f,1.0f,1.0f,1.0f));
    f->glDrawArrays(GL_LINES,0,_gridCountOfVerts);
    _program->release();
}

void MapViewer::resizeGL(int width, int height)
{
   // static float kat = 0.0f;
   // if(kat > 360.0f) kat-=360.0f;
   // kat += 2.0f;
    _projMat.setToIdentity();
    _projMat.perspective(60.0f,(float)(width)/(float)height, 0.01f,100.0f);



}

void MapViewer::mousePressEvent(QMouseEvent * event)
{
    _mouseLastPos = event->pos();
}

void MapViewer::mouseMoveEvent(QMouseEvent * event)
{
    int dx = event->x() - _mouseLastPos.x();
    int dy = event->y() - _mouseLastPos.y();

    if(dy < -2) _cameraAngleX -= 4.0f;
    else if(dy > 2) _cameraAngleX += 4.0f;
    if(dx < -2) _cameraAngleY -= 4.0f;
    else if(dx > 2) _cameraAngleY += 4.0f;

    if(_cameraAngleX > 360.0f) _cameraAngleX -= 360.0f;
    else if(_cameraAngleX < -360.0f) _cameraAngleX += 360.0f;
    if(_cameraAngleY > 360.0f) _cameraAngleY -= 360.0f;
    else if(_cameraAngleY < -360.0f) _cameraAngleY += 360.0f;

    _mouseLastPos = event->pos();
    update();
}

void MapViewer::wheelEvent(QWheelEvent * event)
{
    int delta = event->delta();
    if(delta > 0 && _cameraFar < -0.2f) _cameraFar *= 0.85f;
    else if(delta < 0 && _cameraFar > -10.0f) _cameraFar *= 1.2f;
    update();
}

void MapViewer::addTestTriangle()
{
    _vao.create();
    _vao.bind();

    _vbo.create();
    _gridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.bind();
    _vbo.allocate(&vertices,4*4*sizeof(GLfloat));
        _program->enableAttributeArray(0);
        _program->setAttributeArray(0,GL_FLOAT,0,4);
}

void MapViewer::addGrid(float space, int rows, int cols)
{
    _gridCountOfVerts = (rows*2+cols*2);
    _gridVertices = new GLfloat[_gridCountOfVerts*4];
    int j=0;
    //Najpierw pionowe
    for(float i= -rows/2; i<=(rows/2-1); i+=1.0f, j+=2)
    {
        _gridVertices[(j+0)*4+0] = ((GLfloat)i)*space;           //X1
        _gridVertices[(j+1)*4+0] = ((GLfloat)i)*space;           //X2
        _gridVertices[(j+0)*4+1] = (GLfloat)0.0f;         //Y1
        _gridVertices[(j+1)*4+1] = (GLfloat)0.0f;         //Y2
        _gridVertices[(j+0)*4+2] = ((GLfloat)(-cols/2))*space;   //Z1
        _gridVertices[(j+1)*4+2] = ((GLfloat)(cols/2-1))*space;  //Z2
        _gridVertices[(j+0)*4+3] = 1.0f;
        _gridVertices[(j+1)*4+3] = 1.0f;
    }
    //Następnie poziome
    for(float i= -cols/2; i<=(cols/2-1); i+=1.0f, j+=2)
    {
        _gridVertices[(j+0)*4+0] = ((GLfloat)(-rows/2))*space;   //X1
        _gridVertices[(j+1)*4+0] = ((GLfloat)(rows/2-1))*space;  //X2
        _gridVertices[(j+0)*4+1] = (GLfloat)0.0f;         //Y1
        _gridVertices[(j+1)*4+1] = (GLfloat)0.0f;         //Y2
        _gridVertices[(j+0)*4+2] = ((GLfloat)i)*space;           //Z1
        _gridVertices[(j+1)*4+2] = ((GLfloat)i)*space;           //Z2
        _gridVertices[(j+0)*4+3] = 1.0f;
        _gridVertices[(j+1)*4+3] = 1.0f;
    }

    //Dowiązanie
    _gridVAO.create();
    _gridVAO.bind();

    _gridVBO.create();
    _gridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _gridVBO.bind();
    _gridVBO.allocate(_gridVertices,_gridCountOfVerts*4*sizeof(GLfloat));

        _program->enableAttributeArray(0);
        _program->setAttributeBuffer(0,GL_FLOAT,0,4);
}

void MapViewer::clean()
{
    makeCurrent();
    _vbo.destroy();
    _gridVBO.destroy();
    delete _program;
    _program = 0;
    doneCurrent();
}

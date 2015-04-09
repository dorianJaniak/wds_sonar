#include "mapviewer.h"

#include <QOpenGLContext>
#include <QDebug>


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
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/IdentityS.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/IdentityS.fs");
    _program->bindAttributeLocation("vVertex",0);
    _program->link();
    _program->bind();

    _projMatID = _program->uniformLocation("Mproj");
    _centerMoveMatID = _program->uniformLocation("Mmove");

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

    _vao.bind();
    _program->bind();
    _program->setUniformValue(_projMatID, _projMat);
    _program->setUniformValue(_centerMoveMatID, _centerMoveMat);

    f->glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    _gridVAO.bind();
    _program->bind();
    f->glDrawArrays(GL_LINES,0,_gridCountOfVerts);
    _program->release();
}

void MapViewer::resizeGL(int width, int height)
{
    static float kat = 0.0f;
    if(kat > 360.0f) kat-=360.0f;
    kat += 2.0f;
    _projMat.setToIdentity();
    _projMat.perspective(60.0f,(float)(width)/(float)height, 0.01f,100.0f);

    _centerMoveMat.setToIdentity();
    _centerMoveMat.translate(0.0f,-0.2f,-1.0f);
    _centerMoveMat.rotate(kat, 0.0f, 1.0f, 0.0f);

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

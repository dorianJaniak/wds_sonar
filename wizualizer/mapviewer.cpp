#include "mapviewer.h"

#include <QOpenGLContext>
#include <QDebug>


const GLfloat vertices [] = {
    -0.6f, 0.0f, 0.0f, 1.0f,      //V1
    0.0f, -0.2f, 0.0f, 1.0f,     //V2
    0.0f, 0.0f, 0.0f, 1.0f,       //V3
    -0.3f, -0.4f, 0.0f, 1.0f
};


MapViewer::MapViewer(QWidget * parent) : QOpenGLWidget(parent)
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
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MapViewer::clean);
    initializeOpenGLFunctions();

    glClearColor(0.0f,0.1f,0.4f,1.0f);

    _program = new QOpenGLShaderProgram;
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/IdentityS.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/IdentityS.fs");
    _program->bindAttributeLocation("vVertex",0);
    _program->link();
    _program->bind();

    _projMatID = _program->uniformLocation("MVP");

    addTestTriangle();
    addGrid();
    _projMat.setToIdentity();
  //  _projMat.translate(0,0,-1);
  //  _projMat.perspective(45.0f,1.0f,0.01,100.0);
    _program->release();
}

void MapViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder * vaoB = new QOpenGLVertexArrayObject::Binder(&_vao);
    _program->bind();
    _program->setUniformValue(_projMatID, _projMat);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    _program->release();
    vaoB->release();
    delete vaoB;

    vaoB = new QOpenGLVertexArrayObject::Binder(&_gridVAO);
    _program->bind();
    _program->setUniformValue(_projMatID, _projMat);
    glDrawArrays(GL_LINES,0,24);
    _program->release();
    vaoB->release();
    delete vaoB;
}

void MapViewer::resizeGL(int width, int height)
{
    _projMat.setToIdentity();
    _projMat.perspective(60.0f,GLfloat(width)/height, 0.01f,100.0f);
}

void MapViewer::addTestTriangle()
{
    _vao.create();
    QOpenGLVertexArrayObject::Binder vaoB(&_vao);

    _vbo.create();
    _vbo.bind();
    _vbo.allocate(&vertices,16*sizeof(GLfloat));

    _vbo.bind();
        QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE, 0*sizeof(GLfloat), 0);
    _vbo.release();
    _vao.release();

}

void MapViewer::addGrid(int rows, int cols)
{
    unsigned int countOfElements = (rows*2+cols*2)*3;
    vertices2 = new GLfloat[countOfElements];
    int j=0;
    //Najpierw pionowe
    for(int i= -rows/2; i<=(rows/2-1); i++, j+=2)
    {
        vertices2[(j+0)*3+0] = (GLfloat)i;           //X1
        vertices2[(j+1)*3+0] = (GLfloat)i;           //X2
        vertices2[(j+0)*3+1] = (GLfloat)-0.2;         //Y1
        vertices2[(j+1)*3+1] = (GLfloat)-0.2;         //Y2
        vertices2[(j+0)*3+2] = (GLfloat)(-cols/2);   //Z1
        vertices2[(j+1)*3+2] = (GLfloat)(cols/2-1);  //Z2
      //  qDebug() << vertices[(j+0)*3+0] << vertices[(j+0)*3+1] << vertices[(j+0)*3+2];
     //   qDebug() << vertices[(j+1)*3+0] << vertices[(j+1)*3+1] << vertices[(j+1)*3+2];
    }
    //Następnie poziome
    for(int i= -cols/2; i<=(cols/2-1); i++, j+=2)
    {
        vertices2[(j+0)*3+0] = (GLfloat)(-rows/2);   //X1
        vertices2[(j+1)*3+0] = (GLfloat)(rows/2-1);  //X2
        vertices2[(j+0)*3+1] = (GLfloat)-0.2;         //Y1
        vertices2[(j+1)*3+1] = (GLfloat)-0.2;         //Y2
        vertices2[(j+0)*3+2] = (GLfloat)i;           //Z1
        vertices2[(j+1)*3+2] = (GLfloat)i;           //Z2
     //   qDebug() << vertices[(j+0)*3+0] << vertices[(j+0)*3+1] << vertices[(j+0)*3+2];
     //   qDebug() << vertices[(j+1)*3+0] << vertices[(j+1)*3+1] << vertices[(j+1)*3+2];
    }

    //Dowiązanie
    _gridVAO.create();
    QOpenGLVertexArrayObject::Binder vaoB(&_gridVAO);

    _gridVBO.create();
    _gridVBO.bind();
    _gridVBO.allocate(&vertices2,countOfElements*sizeof(GLfloat));
   // _gridVBO.allocate(&vertices, 9*sizeof(GLfloat));

    _gridVBO.bind();
        QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();                       //Rejestrowanie w oknie
        f->glEnableVertexAttribArray(0);//_gridVAO.objectId());
        f->glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 0*sizeof(GLfloat), 0);
    _gridVBO.release();
    _gridVAO.release();

    //delete [] vertices;
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

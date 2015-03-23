#include "mapviewer.h"

#include <QOpenGLContext>

const GLfloat vertices [] = {
    0.0f, 0.7f, -1.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
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
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shader/IdentityS.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shader/IdentityS.fs");
    _program->bindAttributeLocation("vVertex",0);
    _program->link();
    _program->bind();

    _projMatID = _program->uniformLocation("MVP");
    _vao.create();
    QOpenGLVertexArrayObject::Binder vaoB(&_vao);

    _vbo.create();
    _vbo.bind();
    _vbo.allocate(&vertices,9*sizeof(GLfloat));

        _vbo.bind();
        QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6*sizeof(GLfloat), 0);
        _vbo.release();

    _projMat.setToIdentity();
    _projMat.translate(0,0,-1);
    _program->release();
}

void MapViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoB(&_vao);
    _program->bind();
    _program->setUniformValue(_projMatID, _projMat);

    glDrawArrays(GL_TRIANGLES,0,3);
    _program->release();
}

void MapViewer::resizeGL(int width, int height)
{
    _projMat.setToIdentity();
    _projMat.perspective(45.0f,GLfloat(width)/height, 0.01f,100.0f);
}

void MapViewer::clean()
{
    makeCurrent();
    _vbo.destroy();
    delete _program;
    _program = 0;
    doneCurrent();
}

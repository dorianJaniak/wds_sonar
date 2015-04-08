#include "mapviewer.h"

#include <QOpenGLContext>
#include <QDebug>


const GLfloat vertices [] = {
    -0.6f, 0.0f, 0.0f, 1.0f,      //V1
    0.0f, -0.2f, 0.0f, 1.0f,     //V2
    0.0f, 0.0f, 0.0f, 1.0f,       //V3
    -0.3f, -0.4f, 0.0f, 1.0f
};

/*const GLfloat vertices3 [] = {
    -0.9f, 0.0f, -0.9f, 1.0f,      //V1
    0.9f, 0.0f, -0.9f, 1.0f,     //V2
    -0.9f, 0.0f, -0.8f, 1.0f,       //V3
    0.9f, 0.0f, -0.8f, 1.0f,
    -0.9f, 0.0f, -0.9f, 1.0f,
    -0.9f, 0.0f, 0.9f, 1.0f,
    -0.8f, 0.0f, -0.9f, 1.0f,
    -0.8f, 0.0f, 0.9f, 1.0f
};*/

const GLfloat vertices3 [] = {
    -0.5f, 0.0f, -1.0f, 1.0f,      //V1
    -0.5f, 0.0f, 1.0f, 1.0f,     //V2
    -0.4f, 0.0f, -1.0f, 1.0f,       //V3
    -0.4f, 0.0f, 1.f, 1.0f,
    -0.3f, 0.0f, -1.f, 1.0f,
    -0.3f, 0.0f, 1.f, 1.0f,
    -0.2f, 0.0f, -1.f, 1.0f,
    -0.2f, 0.0f, 1.f, 1.0f
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
   // f = (QOpenGLFunctions*) QOpenGLContext::currentContext()->versionFunctions();
    initializeOpenGLFunctions();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MapViewer::clean);



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
    _program->release();
}

void MapViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    _vao.bind();
    _program->bind();
    _program->setUniformValue(_projMatID, _projMat);

    f->glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    _gridVAO.bind();
    _program->bind();
   // f->glDrawArrays(GL_LINES,0,8);
    f->glDrawArrays(GL_LINES,0,countOfElements/4);
    _program->release();
}

void MapViewer::resizeGL(int width, int height)
{
    static float kat = 0.0f;
    if(kat > 360.0f) kat-=360.0f;
    kat += 2.0f;
    _projMat.setToIdentity();
    _projMat.rotate(kat,1.0f,0.2f,0.0f);
    _projMat.perspective(120.0f,GLfloat(width)/height, 0.1f,100.0f);
}

void MapViewer::addTestTriangle()
{
    _vao.create();
  //  QOpenGLVertexArrayObject::Binder vaoB(&_vao);
    _vao.bind();

    _vbo.create();
    _gridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.bind();
    _vbo.allocate(&vertices,4*4*sizeof(GLfloat));

      //  QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
        _program->enableAttributeArray(0);
        _program->setAttributeArray(0,GL_FLOAT,0,4);
     //   f->glEnableVertexAttribArray(0);
     //   f->glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE, 0*sizeof(GLfloat), 0);
 //   _vbo.release();
 //   _vao.release();

}

void MapViewer::addGrid(int rows, int cols)
{
    countOfElements = (rows*2+cols*2)*4;
    vertices2 = new GLfloat[countOfElements];
    int j=0;
    //Najpierw pionowe
    for(float i= -rows/2; i<=(rows/2-1); i+=1.0f, j+=2)
    {
     //   vertices2[(j+0)*4+0] = (GLfloat)i;           //X1
     //   vertices2[(j+1)*4+0] = (GLfloat)i;           //X2
        vertices2[(j+0)*4+0] = (GLfloat)(i/(float)rows);           //X1
        vertices2[(j+1)*4+0] = (GLfloat)(i/(float)rows);           //X2
        vertices2[(j+0)*4+1] = (GLfloat)0.0f;         //Y1
        vertices2[(j+1)*4+1] = (GLfloat)0.0f;         //Y2
     //   vertices2[(j+0)*4+2] = (GLfloat)(-cols/2);   //Z1
     //   vertices2[(j+1)*4+2] = (GLfloat)(cols/2-1);  //Z2
        vertices2[(j+0)*4+2] = (GLfloat)(-1.0f);   //Z1
        vertices2[(j+1)*4+2] = (GLfloat)(1.0f);  //Z2
        vertices2[(j+0)*4+3] = 1.0f;
        vertices2[(j+1)*4+3] = 1.0f;
        qDebug() << vertices2[(j+0)*4+0] << vertices2[(j+0)*4+1] << vertices2[(j+0)*4+2];
        qDebug() << vertices2[(j+1)*4+0] << vertices2[(j+1)*4+1] << vertices2[(j+1)*4+2];
    }
    //Następnie poziome
    for(float i= -cols/2; i<=(cols/2-1); i+=1.0f, j+=2)
    {
    //    vertices2[(j+0)*4+0] = (GLfloat)(-rows/2);   //X1
    //    vertices2[(j+1)*4+0] = (GLfloat)(rows/2-1);  //X2
        vertices2[(j+0)*4+0] = (GLfloat)(-1.0f);           //X1
        vertices2[(j+1)*4+0] = (GLfloat)(1.0f);           //X2
        vertices2[(j+0)*4+1] = (GLfloat)0.0f;         //Y1
        vertices2[(j+1)*4+1] = (GLfloat)0.0f;         //Y2
    //    vertices2[(j+0)*4+2] = (GLfloat)i;           //Z1
    //    vertices2[(j+1)*4+2] = (GLfloat)i;           //Z2
        vertices2[(j+0)*4+2] = (GLfloat)(i/(float)cols);           //Z1
        vertices2[(j+1)*4+2] = (GLfloat)(i/(float)cols);           //Z2
        vertices2[(j+0)*4+3] = 1.0f;
        vertices2[(j+1)*4+3] = 1.0f;
        qDebug() << vertices2[(j+0)*4+0] << vertices2[(j+0)*4+1] << vertices2[(j+0)*4+2];
        qDebug() << vertices2[(j+1)*4+0] << vertices2[(j+1)*4+1] << vertices2[(j+1)*4+2];
    }

    //Dowiązanie
    _gridVAO.create();
   // QOpenGLVertexArrayObject::Binder vaoB(&_gridVAO);
    _gridVAO.bind();

    _gridVBO.create();
    _gridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _gridVBO.bind();
   // _gridVBO.allocate(&vertices3,8*4*sizeof(GLfloat));
    _gridVBO.allocate(vertices2,countOfElements*sizeof(GLfloat));

        _program->enableAttributeArray(0);
        _program->setAttributeBuffer(0,GL_FLOAT,0,4);
     //   QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();                       //Rejestrowanie w oknie
     //   f->glEnableVertexAttribArray(0);//_gridVAO.objectId());
     //   f->glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE, 0*sizeof(GLfloat), 0);
   // _gridVBO.release();
  //  _gridVAO.release();

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

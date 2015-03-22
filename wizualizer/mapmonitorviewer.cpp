#include "mapmonitorviewer.h"
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <QSurface>


MapMonitorViewer::MapMonitorViewer(QWidget *parent)
{
    _widget = this;
    _context = QOpenGLContext::currentContext();
 /*   QSurfaceFormat format;
    format.setDepthBufferSize(24);          ///Ilość bitów przypadająca na głębię
    format.setStencilBufferSize(8);         ///Bufor obcinania
    format.setVersion(2,0);                 ///wersja OpenGL
    format.setProfile(QSurfaceFormat::CoreProfile); ///To zostanie zignorowane, ponieważ pracuję na starym formacie
    _widget->setFormat(format);
*/
}

MapMonitorViewer::~MapMonitorViewer()
{
    cleanup();
}

void MapMonitorViewer::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MapMonitorViewer::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0.0,0.1,0.3,0.0);
    _program = new QOpenGLShaderProgram(this);
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/main.vs");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/main.fs");
    _program->bindAttributeLocation("posAttr", 0);
    _program->bindAttributeLocation("colAttr",1);
    _program->link();
    _program->bind();
    _attribPos = _program->attributeLocation("posAttr");
    _attribPos = _program->attributeLocation("colAttr");
    _uniformMatrix = _program->uniformLocation("matrix");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_logoVbo.create();
    m_logoVbo.bind();
    //m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    _program->release();
}

void MapMonitorViewer::setupVertexAttribs()
{
    m_logoVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_logoVbo.release();
}

void MapMonitorViewer::resizeGL(int w, int h)
{
  //  glViewport(0,0,w,h);
    m_projection.setToIdentity();
        m_projection.perspective(60.0f, w / float(h), 0.01f, 1000.0f);
}

void MapMonitorViewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawTriangle();
    update();
}


void MapMonitorViewer::drawTriangle()
{
    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    _program->bind();
    QMatrix4x4 mat;
    mat.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    mat.translate(0,0,-2);
    mat.rotate(100.0f * m_frame / 50.0f, 0, 1, 0);
    _program->setUniformValue(_uniformMatrix,mat);

    GLfloat vertices[] = {
        2.0f, 2.707f,
        -2.5f, -2.5f,
        2.5f, -2.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    f->glVertexAttribPointer(_attribPos, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glVertexAttribPointer(_attribCol, 3, GL_FLOAT, GL_FALSE, 0, colors);

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    _program->release();

    ++m_frame;
}

 /*GLuint MapMonitorViewer::loadShader(GLenum type, const char * filename)
 {
     //TODO: musi być wywołanie ładowania pliku
    GLuint shader = glCreateShader(type);
    glShaderSource(shader,1,&filename,0);               //1 ponieważ tylko jeden plik będzie ładowany
    glCompileShader(shader);
    return shader;
 }*/

QSize MapMonitorViewer::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MapMonitorViewer::sizeHint() const
{
    return QSize(400, 400);
}

void MapMonitorViewer::cleanup()
{
    makeCurrent();
    m_logoVbo.destroy();
    delete _program;
    m_program = 0;
    doneCurrent();
}

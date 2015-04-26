#include "mapviewer.h"
#include "envobj.h"
#include "globalvariables.h"

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
    _gridVAO(parent),
    m_parent(parent),
    m_gridColor(g_gridColor),
    m_backgroundColor(g_backgroundColor)
{
    _cameraAngleX = 0.0f;
    _cameraAngleY = 0.0f;
    _cameraFar = -2.0f;
    m_robotMesh = nullptr;
    m_requestRobot = nullptr;
    m_requestRobotVAO = nullptr;
    m_requestRobotVBO = nullptr;
}

MapViewer::~MapViewer()
{
    clean();
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

    glClearColor(m_backgroundColor.x(),m_backgroundColor.y(),m_backgroundColor.z(),m_backgroundColor.w());

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
    addRequestRobot();
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
    _program->setUniformValue(_materialColorID, m_gridColor);
    f->glEnable(GL_LINE_SMOOTH);
    f->glLineWidth(g_gridLineWidth);
    f->glDrawArrays(GL_LINES,0,_gridCountOfVerts);
    f->glDisable(GL_LINE_SMOOTH);

    if(m_requestRobot != nullptr && m_requestRobotVAO != nullptr && m_requestRobotVBO != nullptr)
    {
        m_requestRobotVAO->bind();
        _program->bind();
        QMatrix4x4 requestRobotMat = m_requestRobot->getTranslationMatrix();
        requestRobotMat =  _centerMoveMat * requestRobotMat;
        _program->setUniformValue(_centerMoveMatID, requestRobotMat);
        _program->setUniformValue(_materialColorID, m_requestRobot->getMaterialColor());
        f->glDrawArrays(GL_TRIANGLES,0,m_requestRobot->getAllVertsCount());
    }

    for(int i=0; i<m_mapsVAOs.size(); i++)
    {
        for(int j=0; j<m_mapsVAOs.at(i).size(); j++)
        {
            (m_mapsVAOs[i])[j]->bind();
            _program->bind();
            QMatrix4x4 objMatrix = m_maps[i]->getTranslationMatrix();
            objMatrix =  _centerMoveMat * objMatrix;
            _program->setUniformValue(_centerMoveMatID, objMatrix);
            _program->setUniformValue(_materialColorID,m_maps[i]->getMaterialColor());
            f->glEnable(GL_LINE_SMOOTH);
            f->glLineWidth(g_mapLineWidth);

            f->glDrawArrays(GL_LINE_STRIP,0,m_maps[i]->getVertsCount(j));
            f->glDisable(GL_LINE_SMOOTH);
        }
    }

    _program->release();
}

void MapViewer::resizeGL(int width, int height)
{
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

    if(dy < -2) _cameraAngleX -= 3.5f;
    else if(dy > 2) _cameraAngleX += 3.5f;
    if(dx < -2) _cameraAngleY -= 3.5f;
    else if(dx > 2) _cameraAngleY += 3.5f;

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
    if(delta > 0 && _cameraFar < -0.4f) _cameraFar *= 0.85f;
    else if(delta < 0 && _cameraFar > -20.0f) _cameraFar *= 1.2f;
    update();
}

void MapViewer::addTestTriangle()
{
    _vao.create();
    _vao.bind();

    _vbo.create();
    _gridVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.bind();
    _vbo.allocate(vertices,4*4*sizeof(GLfloat));
        _program->enableAttributeArray(0);
        _program->setAttributeArray(0,GL_FLOAT,0,4);
}

void MapViewer::addGrid(float space, int rows, int cols)
{
    _gridCountOfVerts = (rows*2+cols*2);
    _gridVertices = new GLfloat[_gridCountOfVerts*4];
    int j=0;
    //Najpierw pionowe
    for(float i= -(float)rows/2; i<=((float)rows/2-1); i+=1.0f, j+=2)
    {
        _gridVertices[(j+0)*4+0] = ((GLfloat)i)*space;           //X1
        _gridVertices[(j+1)*4+0] = ((GLfloat)i)*space;           //X2
        _gridVertices[(j+0)*4+1] = (GLfloat)0.0f;         //Y1
        _gridVertices[(j+1)*4+1] = (GLfloat)0.0f;         //Y2
        _gridVertices[(j+0)*4+2] = ((GLfloat)(-(float)cols/2))*space;   //Z1
        _gridVertices[(j+1)*4+2] = ((GLfloat)((float)cols/2-1))*space;  //Z2
        _gridVertices[(j+0)*4+3] = 1.0f;
        _gridVertices[(j+1)*4+3] = 1.0f;
    }
    //Następnie poziome
    for(float i= -(float)cols/2; i<=((float)cols/2-1); i+=1.0f, j+=2)
    {
        _gridVertices[(j+0)*4+0] = ((GLfloat)(-(float)rows/2))*space;   //X1
        _gridVertices[(j+1)*4+0] = ((GLfloat)((float)rows/2-1))*space;  //X2
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

void MapViewer::addEnvMap(QVector<QVector<QVector4D>*> * verts, QVector4D center, bool allowToModifyY)
{
    static int mapNo = 0;
    float upGo = mapNo*g_yMapStep;
    while(upGo>1.0f) upGo -= 1.0f;

    makeCurrent();
    m_maps.push_back(new EnvMap(verts,countColor(mapNo),center+QVector4D(0.0f,upGo,0.0f,0.0f)));

    m_mapsVAOs.push_back(QVector<QOpenGLVertexArrayObject*> ());
    m_mapsVBOs.push_back(QVector<QOpenGLBuffer*> ());

    for(int i=0; i<m_maps.back()->getMeshesCount(); i++)
    {
        QOpenGLVertexArrayObject * refArray = new QOpenGLVertexArrayObject(m_parent);
        refArray->create();
        refArray->bind();
        QOpenGLBuffer * refBuffer = new QOpenGLBuffer();
        refBuffer->create();
        refBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
        refBuffer->bind();
        refBuffer->allocate(m_maps.back()->getVerts(i),m_maps.back()->getVertsCount(i)*4*sizeof(GLfloat));

        _program->enableAttributeArray(0);
        _program->setAttributeBuffer(0,GL_FLOAT,0,4);

        m_mapsVAOs.back().push_back(refArray);
        m_mapsVBOs.back().push_back(refBuffer);
    }
    doneCurrent();

    mapNo++;
}

void MapViewer::addRequestRobot()
{
    if(m_robotMesh == nullptr)
        return;
    if(m_requestRobotVAO != nullptr)
        delete m_requestRobotVAO;
    if(m_requestRobotVBO != nullptr)
        delete m_requestRobotVBO;
    if(m_requestRobot != nullptr)
        delete m_requestRobot;

    QVector<QVector<QVector4D>*>* verts = new QVector<QVector<QVector4D>*>;
    verts->push_back(m_robotMesh);
    m_requestRobot = new EnvObj(verts,g_requestRobotColor,QVector4D(0,0,0,1),g_requestRobotScale);

    makeCurrent();
    m_requestRobotVAO = new QOpenGLVertexArrayObject(m_parent);
    m_requestRobotVAO->create();
    m_requestRobotVAO->bind();
    m_requestRobotVBO = new QOpenGLBuffer();
    m_requestRobotVBO->create();
    m_requestRobotVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_requestRobotVBO->bind();
    m_requestRobotVBO->allocate(m_requestRobot->getVerts(0),m_requestRobot->getAllVertsCount()*4*sizeof(GLfloat));
    _program->enableAttributeArray(0);
    _program->setAttributeBuffer(0,GL_FLOAT,0,4);

    doneCurrent();


}

void MapViewer::setRobotMesh(QVector<QVector4D>* verts)
{
    if(m_robotMesh != nullptr && verts!=nullptr)
        delete m_robotMesh;
    m_robotMesh = verts;
}

void MapViewer::setRequestRobotOrientation(QVector4D position, float angleY)
{
    if(m_requestRobot != nullptr)
    {
        m_requestRobot->setCenter(position);
        m_requestRobot->setAngle(angleY);
    }
}

void MapViewer::clean()
{
    makeCurrent();
    _vbo.destroy();
    _gridVBO.destroy();
    _vao.destroy();
    _gridVAO.destroy();
    m_maps.clear();
    m_mapsVAOs.clear();
    m_mapsVBOs.clear();
    delete _program;
    _program = 0;
    doneCurrent();
}

QVector4D MapViewer::countColor(int objIndex)
{
    float toAddColor = g_colorStep*objIndex;
    if(toAddColor > (3.0f-g_startMapColor.x()+g_startMapColor.y()+g_startMapColor.z()))
        objIndex = 0;
    QVector4D color(g_startMapColor);
    for(int i=2; i>=0; i--)
        if( 1.0f - color[i] < toAddColor )
        {
            toAddColor -= (1.0f-color[i]);
            color[i] += (1.0f-color[i]);
        }
        else
        {
            color[i] += toAddColor;
            toAddColor = 0.0f;
        }
    return color;
}

#include "envobj.h"

EnvObj::EnvObj(QVector<QVector<QVector4D>*>* verts, QVector4D color, QVector4D center, float angleY, QVector4D scale) :
    m_centerPos(center),
    m_colorMaterial(color),
    m_scale(scale),
    m_angleY(angleY)
{
    m_allVertsCount = 0;
    for(int i=0; i<verts->size(); i++)
    {
        QVector<QVector4D> * vertsPtr = verts->at(i);
        m_verticesCount.push_back(vertsPtr->size());
        m_allVertsCount += vertsPtr->size();
        float * vertsPart = new GLfloat[m_verticesCount.back()*4];

        for(int j=0; j<m_verticesCount.back(); j++)
        {
            vertsPart[j*4] = vertsPtr->at(j).x();
            vertsPart[j*4+1] = vertsPtr->at(j).y();
            vertsPart[j*4+2] = vertsPtr->at(j).z();
            vertsPart[j*4+3] = vertsPtr->at(j).w();
            qDebug() << vertsPart[j*4] << vertsPart[j*4+1] << vertsPart[j*4+2] << vertsPart[j*4+3];
        }

        m_vertices.push_back(vertsPart);
    }
}

EnvObj::~EnvObj()
{

}

unsigned int EnvObj::getMeshesCount()
{
    return m_vertices.size();
}

const float * EnvObj::getVerts(int meshIndex)
{
    if(m_vertices.size()>meshIndex)
        return m_vertices[meshIndex];
    return nullptr;
}

unsigned int EnvObj::getVertsCount(int meshIndex)
{
    if(m_verticesCount.size()>meshIndex)
        return m_verticesCount[meshIndex];
    return 0;
}

unsigned int EnvObj::getAllVertsCount()
{
    return m_allVertsCount;
}

QMatrix4x4 EnvObj::getTranslationMatrix()
{
    QMatrix4x4 result;
    result.setToIdentity();

    result.translate(m_centerPos.x(),m_centerPos.y(),m_centerPos.z());
    result.rotate(m_angleY+90.0f,0.0f,1.0f,0.0f);
    result.scale(m_scale.x(),m_scale.y(),m_scale.z());

    return result;
}

QVector4D EnvObj::getMaterialColor()
{
    return m_colorMaterial;
}

void EnvObj::setCenter(QVector4D position)
{
    m_centerPos = position;
}

void EnvObj::setAngle(float angleY)
{
    m_angleY = angleY;
}

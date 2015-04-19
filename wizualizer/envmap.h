#ifndef ENVMAP_H
#define ENVMAP_H

#include<QVector4D>
#include<QMatrix4x4>
#include<QVector>
#include <QOpenGLFunctions>

class EnvMap
{
    QVector<const GLfloat*> m_vertices;
    QVector<unsigned int> m_verticesCount;
    unsigned int m_allVertsCount;
    QVector4D m_centerPos;

public:
    EnvMap(QVector<QVector<QVector4D>*>* verts, QVector4D center = QVector4D(0.0f,0.0f,0.0f,1.0f));
    ~EnvMap();
    unsigned int getMeshesCount();
    const float * getVerts(int meshIndex);
    unsigned int getVertsCount(int meshIndex);
    unsigned int getAllVertsCount();
    QMatrix4x4 getTranslationMatrix();

};

#endif // ENVMAP_H

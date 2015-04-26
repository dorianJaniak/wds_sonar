#include "filecontroller.h"
#include "globalvariables.h"

#include <QFile>
#include <QDebug>

FileController::FileController(QObject *parent) : m_parent(parent)
{

}

FileController::~FileController()
{

}

QVector<QVector<QVector4D>*> *FileController::loadSensorDataFromFile(const QString & fileName)
{
    QStringList allFields = getFromCSVFile(fileName);
    return reinterpretW00(allFields);
}

QStringList FileController::getFromCSVFile(const QString & fileName)
{
    QStringList allFields;
    QFile file(fileName);
    if(!file.exists())
        return allFields;
    if(file.open(QFile::ReadOnly))
    {
        QString allData = file.readAll();
        emit sendLog(tr("WIADOMOŚĆ: ") + allData);
        allFields = allData.split(';');
        file.close();
    }
    return allFields;
}

QVector<QVector4D>* FileController::getFromSTLFile(const QString & fileName)
{
    QStringList allFields;
    QFile file(fileName);
    if(!file.exists())
        return nullptr;
    if(file.open(QFile::ReadOnly))
    {
        QVector<QVector4D>* stlMesh = new QVector<QVector4D>;
        QString allData = file.readAll();
        allFields = allData.split('\n');
        file.close();
        for(int i=0; i<allFields.size(); i++)
        {
            if(allFields.at(i).contains("vertex"))
            {
                QStringList vFields = allFields.at(i).split(' ');
                QVector4D oneMesh(vFields.at(1).toFloat(),vFields.at(2).toFloat(),vFields.at(3).toFloat(),1.0f);
                stlMesh->push_back(oneMesh);
            }
        }
        emit sendLog(tr("Udało się załadować obiekt 3D przedstawiający robota - robot.stl"));
        return stlMesh;
    }
}

#include "filecontroller.h"
#include "globalvariables.h"

#include <QFile>
#include <QDebug>

FileController::FileController()
{

}

FileController::~FileController()
{

}

QVector<QVector4D> *FileController::loadSensorDataFromFile(const QString & fileName)
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
        allFields = allData.split(';');
        file.close();
    }
    return allFields;
}

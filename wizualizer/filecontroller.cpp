#include "filecontroller.h"

#include <QFile>
#include <QDebug>

FileController::FileController()
{

}

FileController::~FileController()
{

}

float * FileController::getFromCSVFile(const QString & fileName)
{
    QFile file(fileName);
    if(!file.exists())
        return nullptr;
    if(file.open(QFile::ReadOnly))
    {
        QString allData = file.readAll();
        QStringList allFields = allData.split(';');
        file.close();

        for(int i=0; i<allFields.size(); i++)
            qDebug() << allFields.at(i);
    }
    else
        return nullptr;



}

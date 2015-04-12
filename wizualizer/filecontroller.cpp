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


}

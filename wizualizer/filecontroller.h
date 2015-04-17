#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QString>
#include "messagecontroller.h"

class QVector4D;

class FileController : public MessageController
{
public:
    FileController();
    ~FileController();

    QVector<QVector4D> * loadSensorDataFromFile(const QString & fileName);
    static QStringList getFromCSVFile(const QString & fileName);
};

#endif // FILECONTROLLER_H

#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QString>
#include "messagecontroller.h"

class QVector4D;

class FileController : public MessageController
{
    Q_OBJECT
    QObject * m_parent;
public:
    FileController(QObject * parent = 0);
    ~FileController();

    QVector<QVector4D> * loadSensorDataFromFile(const QString & fileName);
    QStringList getFromCSVFile(const QString & fileName);
};

#endif // FILECONTROLLER_H

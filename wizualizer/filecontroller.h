#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QString>

class FileController
{
public:
    FileController();
    ~FileController();
    static float *getFromCSVFile(const QString & fileName);
};

#endif // FILECONTROLLER_H

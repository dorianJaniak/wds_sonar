#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QString>
#include <QVector>
#include <QObject>


class QVector4D;

enum MessageTypes {
    w00_map_data = 0
};

enum ErrorType {
    e00_problem_header = 0,
    e01_problem_params,
    e02_lost_param,

    e_no_error
};

class MessageController : public QObject
{
    Q_OBJECT
    static QString m_wNames[];
public:


    MessageController();
    ~MessageController();
    QVector<QVector4D> * reinterpretW00(QStringList & allFields);

signals:
    void sendLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());

};

#endif // MESSAGECONTROLLER_H

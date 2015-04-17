#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QString>
#include <QVector>
#include <QCoreApplication>
class QVector4D;



enum MessageTypes {
    w00_map_data = 0
};
enum ErrorType {
    e00_lost_parametr = 0,
    e01_no_header,
    e02_wrong_header,
    e03_wrong_count_of_args,

    e_no_error,
    e_no_error_with_this_index
};

class MessageStateInfo {
    Q_DECLARE_TR_FUNCTIONS(MessageStateInfo)

public:
    static QString m_eNames[];

    MessageStateInfo(MessageTypes type);
    MessageStateInfo() {};          //TO TRZA SPRAWDZIC
    void addError(ErrorType type);
    ErrorType getError(int indeks);
    QString& getErrorCaption(int indeks);
    QStringList getErrorsCaptions();
    unsigned int getErrorsCount();
    unsigned int getID();

private:
    QVector<ErrorType> m_errors;
    static unsigned int m_counter;
    unsigned int m_id;
    MessageTypes m_messType;
};

class MessageController
{
    QVector<MessageStateInfo> m_states;
    static QString m_wNames[];
public:


    MessageController();
    ~MessageController();
    QVector<QVector4D> * reinterpretW00(QStringList & allFields);
};

#endif // MESSAGECONTROLLER_H

#include "messagecontroller.h"
#include <QVector4D>
#include <QStringList>
#include <qmath.h>

QString MessageController::m_wNames[] = {
    QString("W0")
};

QString MessageStateInfo::m_eNames[] = {
    QString(tr("brak lub błędny parametr wiadomości")),
    QString(tr("brak nagłówka wiadomości")),
    QString(tr("błędny nagłówek wiadomości")),
    QString(tr("błędna ilość parametrów wiadomości"))
};

unsigned int MessageStateInfo::m_counter = 0;

MessageStateInfo::MessageStateInfo(MessageTypes type)
{
    m_id = m_counter++;
    m_messType = type;
    m_errors.push_back(e_no_error);
}

void MessageStateInfo::addError(ErrorType type)
{
    m_errors.push_back(type);
}

ErrorType MessageStateInfo::getError(int indeks)
{
    if(indeks >= m_errors.size())
        return e_no_error_with_this_index;
    return m_errors.at(indeks);
}

QString& MessageStateInfo::getErrorCaption(int indeks)
{
    return m_eNames[getError(indeks)];
}

QStringList MessageStateInfo::getErrorsCaptions()
{
    QStringList errorsList;
    for(int i=0;i<m_errors.size();i++)
        errorsList.push_back(getErrorCaption(i));
    return errorsList;
}

unsigned int MessageStateInfo::getErrorsCount()
{
    return m_errors.size();
}

unsigned int MessageStateInfo::getID()
{
    return m_id;
}

MessageController::MessageController()
{

}

MessageController::~MessageController()
{

}

QVector<QVector4D> * MessageController::reinterpretW00(QStringList & allFields)
{
  //  MessageStateInfo(MessageTypes::w00_map_data);
    m_states.push_back(MessageStateInfo(MessageTypes::w00_map_data));
    if(allFields.size()<=1)                     ///Gdy wiadomosc nie posiada naglowka i ilosci argumentow
    {
        m_states.back().addError(ErrorType::e01_no_header);
        return nullptr;
    }
    if(allFields.at(0)!=m_wNames[MessageTypes::w00_map_data])   ///Gdy wiadomosc nie posiada naglowka
    {
        m_states.back().addError(ErrorType::e02_wrong_header);
        return nullptr;
    }
    bool ok;
    unsigned int countOfParams = allFields.at(1).toInt(&ok);
    if(!ok)
    {
        m_states.back().addError(ErrorType::e02_wrong_header);
        return nullptr;
    }
    if(countOfParams + 2 != allFields.size())   ///Gdy odczytana ilosc argumentow nie zgadza sie z dlugoscia otrzymanej wiadomosci
    {
       m_states.back().addError(ErrorType::e03_wrong_count_of_args);
        return nullptr;
    }

    QVector<QVector4D> * data = new QVector<QVector4D>;
    data->reserve(countOfParams/2);
    for(unsigned int i=0; i<countOfParams; i+=2)
    {
        bool ok_part;
        float angle = allFields.at(i).toFloat(&ok);
        float far = allFields.at(i+1).toFloat(&ok_part);

        if(ok_part && ok)
        {
            float x = far*sin(angle);
            float z = -far*cos(angle);
            (*data)[i/2] = QVector4D(x,0.0f,-z,1.0f);
        }
        else
        {
            m_states.back().addError(ErrorType::e00_lost_parametr);
            (*data)[i/2] = QVector4D(0.0f,0.0f,0.0f,0.0f);
        }
    }
    return data;
}

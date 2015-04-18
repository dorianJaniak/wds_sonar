#include "messagecontroller.h"
#include <QVector4D>
#include <QStringList>
#include <qmath.h>

QString MessageController::m_wNames[] = {
    QString("W0"),
    QString("W1")
};



MessageController::MessageController()
{

}

MessageController::~MessageController()
{

}

QVector<QVector4D> * MessageController::reinterpretW00(QStringList & allFields)
{

    QVector<ErrorType> errors;
    if(allFields.size()<=1)                     ///Gdy wiadomosc nie posiada naglowka i ilosci argumentow
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(tr("Problemy z odczytem wiadomości W00 "),errors);
        return nullptr;
    }
    if(allFields.at(0)!=m_wNames[MessageTypes::w00_map_data])   ///Gdy wiadomosc nie posiada naglowka
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(tr("Problemy z odczytem wiadomości W00 "),errors);
        return nullptr;
    }
    bool ok;
    unsigned int countOfParams = allFields.at(1).toInt(&ok);
    if(!ok)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(tr("Problemy z odczytem wiadomości W00 "),errors);
        return nullptr;
    }
    if(countOfParams + 2 != allFields.size())   ///Gdy odczytana ilosc argumentow nie zgadza sie z dlugoscia otrzymanej wiadomosci
        errors.push_back(ErrorType::e01_problem_params);

    QVector<QVector4D> * data = new QVector<QVector4D>;
    for(unsigned int i=2; i<allFields.size(); i+=2)
    {
        bool ok_part;
        float angle = allFields.at(i).toFloat(&ok);
        float far = allFields.at(i+1).toFloat(&ok_part);

        if(ok_part && ok)
        {
            float x = far*sin(angle)/100.0f;
            float z = -far*cos(angle)/100.0f;
            data->push_back(QVector4D(x,0.0f,-z,1.0f));
        }
        else
        {
            errors.push_back(ErrorType::e02_lost_param);
            data->push_back(QVector4D(0.0f,0.0f,0.0f,0.0f));
        }
    }
    if(errors.size() > 0)
        emit sendLog(tr("Problemy z odczytem wiadomości W00 "),errors);
    return data;
}


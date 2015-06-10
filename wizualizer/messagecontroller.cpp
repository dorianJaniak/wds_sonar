#include "messagecontroller.h"
#include "globalvariables.h"
#include <QVector4D>
#include <QStringList>
#include <qmath.h>

QString MessageController::m_wNames[] = {
    QString("W0"),
    QString("W1"),
    QString("W2"),
    QString("W3"),
    QString("W4"),
    QString("W5"),
    QString("W6"),
    QString("W99")
};



MessageController::MessageController()
{

}

MessageController::~MessageController()
{

}

MessageTypes MessageController::checkMessageType(const QString & msg)
{
    if(msg.size()<2)
        return w_none;
    for(unsigned i=0; i<9; i++)
        if(msg.contains(m_wNames[i]))
            return (MessageTypes)i;
    return w_none;
}

QString MessageController::prepareStringFromList(QStringList list)
{
    QString result;
    if(list.size()>0)
    {
        result = list.at(0);
        for(int i=1; i<list.size(); i++)
            result += (QString(";")+list.at(i));
    }
    return result;
}

QStringList MessageController::prepareListFromString(QString msg)
{
    return msg.split(';');
}

bool MessageController::reinterpretW00(QStringList allFields)
{
    QString caption = tr("Problemy z odczytem wiadomości W00 ");
    QVector<ErrorType> errors;
    if(allFields.size()!=2)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    if(checkMessageType(allFields.at(0)) != w_ok)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    bool ok;
    unsigned int countOfParams = allFields.at(1).toInt(&ok);
    if(!ok)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    if(countOfParams!=0)
    {
        errors.push_back(ErrorType::e01_problem_params);
        emit sendLog(caption,errors);
        return nullptr;
    }
    return true;
}

bool MessageController::reinterpretW01(QStringList & allFields)
{
    QString caption = tr("Problemy z odczytem wiadomości W01 ");
    QVector<ErrorType> errors;
    if(allFields.size()!=2)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    if(checkMessageType(allFields.at(0)) != w_start)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    bool ok;
    unsigned int countOfParams = allFields.at(1).toInt(&ok);
    if(!ok)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    if(countOfParams!=0)
    {
        errors.push_back(ErrorType::e01_problem_params);
        emit sendLog(caption,errors);
        return nullptr;
    }
    return true;
}

QStringList MessageController::prepareW02(unsigned cMeasurements, unsigned vDegreeSpeed)
{
    QStringList result;
    result.append(m_wNames[w_scan]);
    result.append(QString::number(2));
    result.append(QString::number(cMeasurements));
    result.append(QString::number(vDegreeSpeed));
    return result;
}

QVector<QVector<QVector4D>*> * MessageController::reinterpretW03(QStringList & allFields)
{
    QString caption = tr("Problemy z odczytem wiadomości W03 ");
    QVector<ErrorType> errors;
    if(allFields.size()<=1)                     ///Gdy wiadomosc nie posiada naglowka i ilosci argumentow
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    if(checkMessageType(allFields.at(0)) != w_map_data)   ///Gdy wiadomosc nie posiada naglowka
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    bool ok;
    unsigned int countOfParams = allFields.at(1).toInt(&ok);
    if(!ok)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return nullptr;
    }
    if(countOfParams + 2 != allFields.size())   ///Gdy odczytana ilosc argumentow nie zgadza sie z dlugoscia otrzymanej wiadomosci
        errors.push_back(ErrorType::e01_problem_params);

    QVector<QVector<QVector4D> *> * data = new QVector<QVector<QVector4D>*>;
    data->push_back(new QVector<QVector4D>);
    bool lastTimeAdded = true;                  ///Jeśli został ostatnio dodany wektor nowej siatki
    for(unsigned int i=2; i<allFields.size(); i+=2)
    {
        bool ok_part;
        float angle = allFields.at(i).toFloat(&ok);
        float far = allFields.at(i+1).toFloat(&ok_part);
        if(far >= g_maxFar && !lastTimeAdded)
        {
            data->push_back(new QVector<QVector4D>);
            lastTimeAdded = true;
        }

        if( far < g_maxFar )
        {
            if( ok_part && ok )
            {
                float x = far*cos((angle)*3.14f/180.0f)/100.0f;
                float z = -far*sin((angle)*3.14f/180.0f)/100.0f;
                data->back()->push_back(QVector4D(x,0.0f,-z,1.0f));
                lastTimeAdded = false;
            }
            else
            {
                errors.push_back(ErrorType::e02_lost_param);
               // data->push_back(QVector4D(0.0f,0.0f,0.0f,0.0f));
            }
        }
    }
    if(errors.size() > 0)
        emit sendLog(caption,errors);
    return data;
}

QStringList MessageController::prepareW04(bool directionRight, unsigned angle, unsigned vDegreeSpeed)
{
    QStringList result;
    result.append(m_wNames[w_calibrate]);
    result.append(QString::number(3));
    result.append(QString::number((unsigned)directionRight));
    result.append(QString::number(angle));
    result.append(QString::number(vDegreeSpeed));
    return result;
}

QStringList MessageController::prepareW05(unsigned vCMperSspeed, unsigned relDistCM, int relRotBM, int relRotAM)
{
    QStringList result;
    result.append(m_wNames[w_move]);
    result.append(QString::number(4));
    result.append(QString::number(vCMperSspeed));
    result.append(QString::number(relDistCM));
    result.append(QString::number(relRotBM));
    result.append(QString::number(relRotAM));
    return result;
}

QVector<int> MessageController::reinterpretW06(QStringList allFields)
{
    QString caption = tr("Problemy z odczytem wiadomości W06 ");
    QVector<ErrorType> errors;
    if(allFields.size()<=1)                     ///Gdy wiadomosc nie posiada naglowka i ilosci argumentow
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return QVector<int>();
    }
    if(checkMessageType(allFields.at(0)) != w_resultMove)   ///Gdy wiadomosc nie posiada naglowka
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return QVector<int>();
    }
    bool ok;
    unsigned int countOfParams = allFields.at(1).toInt(&ok);
    if(!ok)
    {
        errors.push_back(ErrorType::e00_problem_header);
        emit sendLog(caption,errors);
        return QVector<int>();
    }
    if(countOfParams + 2 != allFields.size())   ///Gdy odczytana ilosc argumentow nie zgadza sie z dlugoscia otrzymanej wiadomosci
        errors.push_back(ErrorType::e01_problem_params);

    QVector<int> result;
    bool ok_part = true;
    if(allFields.size() > 4)
    {
        result.push_back(allFields.at(2).toInt(&ok));
        ok_part &= ok;
        result.push_back(allFields.at(3).toInt(&ok));
        ok_part &= ok;
        result.push_back(allFields.at(4).toInt(&ok));
        ok_part &= ok;
    }
    else
    {
        errors.push_back(ErrorType::e01_problem_params);
    }
    if(!ok_part)
        errors.push_back(ErrorType::e02_lost_param);

    if(errors.size() > 0)
    {
        emit sendLog(caption,errors);
        return QVector<int>();
    }
    return result;
}

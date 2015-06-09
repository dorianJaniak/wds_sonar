#include "robotcontroller.h"
#include <QtSerialPort/QSerialPortInfo>


RobotController::RobotController() :
    MessageController()
 //   QSerialPort()
{
    RobotOrientation empty;
    empty.position = QVector4D(0,0,0,1);
    empty.angleY = 0;
    m_robotActualOrient = empty;
}

RobotController::~RobotController()
{

}

RobotOrientation RobotController::getActualOrientation()
{
    return m_robotActualOrient;
}

RobotOrientation RobotController::getExpectedOrientation()
{
    return m_robotExpectedOrient;
}

void RobotController::moveRobot(RobotOrientation displacement)
{
    m_robotExpectedOrient.angleY += displacement.angleY;
    for(int i=0; i<3; i++)
        m_robotExpectedOrient.position[i] += displacement.position[i];
}

bool RobotController::moveStepperMotor(bool directionRight, unsigned angle, unsigned vDegreeSpeed)
{
    QString msg_send = prepareStringFromList(prepareW04(directionRight,angle,vDegreeSpeed));
    serial.write(msg_send.toLocal8Bit());
    unsigned timeMS = (angle*1000)/vDegreeSpeed;
    emit blockWindow(tr("Poczekaj %1 s, powinieneś zobaczyć obracający się czujnik zamieszczony na silniku krokowym.").arg(QString::number(timeMS/1000)),timeMS);
/*
            QString msg_response(serial.readLine());
            emit sendLog(tr("WIADOMOŚĆ: ") + msg_response);
            if(reinterpretW00(prepareListFromString(msg_response)))
            {
                emit sendLog(tr("Operacja obrotu sonaru zakończona powodzeniem."));
                return true;
            }
     //   }

    return false;
    */
    return true;
}

void RobotController::initializeSerial()
{
    serial.setBaudRate(9600,QSerialPort::AllDirections);
    serial.setDataBits(QSerialPort::Data8);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if(ports.size())
        serial.setPort(ports.at(0));
    else
        serial.setPortName("");
    serial.setStopBits(QSerialPort::OneStop);
}

bool RobotController::openSerial()
{
    QString caption = MessageController::tr("Problem COM %1:").arg(serial.portName());
    QVector<ErrorType> errors;
    if(serial.portName().isEmpty())
    {
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        errors.append(e13_serial_not_configured);
        if(ports.size())
        {
            serial.setPort(ports.at(0));
        }
        else
        {
            errors.append(e10_serial_not_opened);
            emit sendLog(caption,errors);
            return false;
        }
    }
    if(!serial.open(QSerialPort::ReadWrite))
    {
        errors.append(e10_serial_not_opened);
        emit sendLog(caption,errors);
        return false;
    }
    emit sendLog(tr("Udało się połączyć z portem: %1").arg(serial.portName()));
    return true;
}

bool RobotController::closeSerial()
{
    serial.close();
    emit sendLog(tr("Port %1 zamknięty").arg(serial.portName()));
    return true;
}

void RobotController::reconfigureSerial(const QSerialPortInfo& port, QSerialPort::BaudRate br, QSerialPort::DataBits db, QSerialPort::FlowControl fc, QSerialPort::Parity p, QSerialPort::StopBits sb)
{
    serial.setBaudRate(br,QSerialPort::AllDirections);
    serial.setDataBits(db);
    serial.setFlowControl(fc);
    serial.setParity(p);
    serial.setStopBits(sb);
    serial.setPort(port);
}

QString RobotController::getInfoAboutSerial()
{
    QString info;
    info += tr("Port: %1\n").arg(serial.portName());
    info += tr("Bodów: %1\n").arg(QString::number(serial.baudRate()));

    QSerialPort::DataBits db = serial.dataBits();
    info += tr("Bity danych: ");
    switch(db)
    {
        case QSerialPort::Data7 : info += "8"; break;
        case QSerialPort::Data8 : info += "7"; break;
        default: info += "Unknown"; break;
    }
    info += "\n";

    QSerialPort::FlowControl fc = serial.flowControl();
    info += tr("Kontrola przepływu: ");
    switch(fc)
    {
        case QSerialPort::NoFlowControl : info += "None"; break;
        case QSerialPort::HardwareControl : info += "RTS/CTS"; break;
        case QSerialPort::SoftwareControl : info += "XON/XOFF"; break;
        default: info += "Unknown"; break;
    }
    info += "\n";

    QSerialPort::Parity p = serial.parity();
    info += tr("Parzystość: ");
    switch(p)
    {
        case QSerialPort::NoParity : info += "None"; break;
        case QSerialPort::EvenParity : info += "Even"; break;
        case QSerialPort::OddParity : info += "Odd"; break;
        case QSerialPort::SpaceParity : info += "Space"; break;
        case QSerialPort::MarkParity : info += "Mark"; break;
        default: info += "Unknown"; break;
    }
    info += "\n";

    QSerialPort::StopBits sb = serial.stopBits();
    info += tr("Bity stopu: ");
    switch(sb)
    {
        case QSerialPort::OneStop : info += "1"; break;
        case QSerialPort::TwoStop : info += "2"; break;
        default: info += "Unknown"; break;
    }
    info += "\n";

    return info;
}

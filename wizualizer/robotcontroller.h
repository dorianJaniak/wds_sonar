#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QVector4D>
#include "messagecontroller.h"
#include "bluetoothcontroller.h"
#include "globalvariables.h"
#include <QtSerialPort/QSerialPort>
#include <QObject>

/**
 * @brief Struktura RobotOrientation
 *
 * Struktura przechowuje informacje o pozycji robota
 */
struct RobotOrientation {
    QVector4D position;         ///<Położenie robota na mapie w stabilnych współrzędnych X,Y,Z,W
    float angleY;               ///<Obrót wokół własnej osi robota
};

/**
 * @brief Klasa obsługująca robota
 *
 * Klasa może obsługiwać robota. Zawiera informacje o jego położeniu,
 * umie się z nim łączyć oraz umie obsługiwać komunikaty przez niego wysyłane.
 */
class RobotController : public MessageController
{
    Q_OBJECT
    QSerialPort serial;
                        ///Wysterowana realnie pozycja (na podstawie enkoderów i algorytmu)
    RobotOrientation m_robotActualOrient;
                         ///Żądana pozycja dla robota (zatwierdzona w programie)
    RobotOrientation m_robotExpectedOrient;
public:
    RobotController();
    ~RobotController();

                        /**
     * @brief Pobiera prawdziwą aktualną pozycję robota
     * @return aktualna pozycja robota we współrzędnych jednorodnych
     */
    RobotOrientation getActualOrientation();
                        /**
    * @brief Pobiera żądaną pozycję robota
    * @return oczekiwana pozycja robota we współrzędnych jednorodnych
    */
    RobotOrientation getExpectedOrientation();
                        /**
     * @brief Obsługa żądania przemieszczenia robota
     *
     * Wywołanie funkcji wiąże się z wysłaniem odpowiednich ramek do robota, a
     * więc też z jego przemieszczeniem. Robot będzie próbował się przemieścić
     * do zadanej pozycji. Przy okazji dane zostaną zapisane w zmiennej m_robotExpectedOrient.
     * @param displacement Przemieszczenie robota we współrzędnych mapy.
     */
    void moveRobot(RobotOrientation displacement);

    bool moveStepperMotor(bool directionRight, unsigned angle, unsigned vDegreeSpeed);

    bool openSerial();
    bool closeSerial();
    QString getInfoAboutSerial();

signals:
    void unlockWindow();
    void blockWindow(QString message, unsigned timeMS);

public slots:
    void reconfigureSerial(const QSerialPortInfo& port, QSerialPort::BaudRate br, QSerialPort::DataBits db, QSerialPort::FlowControl fc, QSerialPort::Parity p, QSerialPort::StopBits sb);

private:
    void initializeSerial();
    void clearSerialBuffers();
    QString readMessageFromSerial(unsigned timeMS);
};

#endif // ROBOTCONTROLLER_H

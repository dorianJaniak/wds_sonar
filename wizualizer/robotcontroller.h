#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QVector4D>
#include "messagecontroller.h"
#include "bluetoothcontroller.h"
#include "globalvariables.h"

struct RobotOrientation {
    QVector4D position;         ///Położenie robota na mapie w stabilnych współrzędnych X,Y,Z,W
    float angleY;               ///Obrót wokół własnej osi robota
};

class RobotController : public MessageController, public BluetoothController
{
    RobotOrientation m_robotActualOrient;             ///Wysterowana realnie pozycja (na podstawie enkoderów i algorytmu)
    RobotOrientation m_robotExpectedOrient;           ///Żądana pozycja dla robota (zatwierdzona w programie)
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

};

#endif // ROBOTCONTROLLER_H

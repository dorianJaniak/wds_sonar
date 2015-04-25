#include "robotcontroller.h"

RobotController::RobotController() : MessageController(), BluetoothController()
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

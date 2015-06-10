#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>

enum MsgPriorityType {
    simpleText = 0,
    warningText = 1,
    errorText = 2
};

const QString MsgPriorityColors[] = {
    QString("<font color=\"black\">"),
    QString("<font color=\"#CC9900\">"),
    QString("<font color=\"red\">")
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_fileController(parent),
    m_buttonHoldTimer(nullptr),
    comWindow(this),
    author(this)
{
    ui->setupUi(this);
    m_monitor = ui->mainView;

    connect(&m_fileController,SIGNAL(sendLog(QString,QVector<ErrorType>)),this,SLOT(showLog(QString,QVector<ErrorType>)));
    connect(&m_robotController,SIGNAL(sendLog(QString,QVector<ErrorType>)),this,SLOT(showLog(QString,QVector<ErrorType>)));
    connect(&comWindow,SIGNAL(updateSerial(const QSerialPortInfo&,QSerialPort::BaudRate,QSerialPort::DataBits,QSerialPort::FlowControl,QSerialPort::Parity,QSerialPort::StopBits)),
            &m_robotController,SLOT(reconfigureSerial(QSerialPortInfo,QSerialPort::BaudRate,QSerialPort::DataBits,QSerialPort::FlowControl,QSerialPort::Parity,QSerialPort::StopBits)));
    connect(&m_robotController,SIGNAL(blockWindow(QString,uint)),this,SLOT(blockWindow(QString,uint)));
    connect(&m_robotController,SIGNAL(unlockWindow()),this,SLOT(unlockWindow()));

    initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    m_moveType = MoveType::stop;
    m_diffOrientation.position = QVector4D(0,0,0,0);
    m_diffOrientation.angleY = 0.0f;

    QVector<QVector4D> * robotPtr = m_fileController.getFromSTLFile("objects/robot.stl");
    if(robotPtr == nullptr)
        QMessageBox::critical(this,tr("Błąd odczytu modelu 3D robota"),tr("Nie udało się załadować pliku robot.stl zawierającego siatkę obiektu prezentującego robota w widoku 3D"));
    else
        m_monitor->setRobotMesh(robotPtr);
}

void MainWindow::on_actionLoadFromSimFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Wczytaj plik symulacyjny"),".",tr("Plik symulacyjny (*)"));
    if(fileName.isEmpty())
        return;
    QVector<QVector<QVector4D>*> * verts = m_fileController.loadSensorDataFromFile(fileName);
    if(verts==nullptr)
        QMessageBox::critical(this,tr("Błąd odczytu mapy"),tr("Nie udało się załadować pliku symulacyjnego"));
    else
    {
        RobotOrientation orientation = m_robotController.getExpectedOrientation();
        m_monitor->addEnvMap(verts,orientation.position, orientation.angleY);
        delete verts;
    }
}

void MainWindow::unlockWindow()
{
    m_blockWindowTimer->stop();
    ui->menuBar->setEnabled(true);
    ui->ControlWidget->setEnabled(true);
    delete m_blockWindowTimer;
}

void MainWindow::blockWindow(QString message, unsigned timeMS)
{
    ui->menuBar->setEnabled(false);
    ui->ControlWidget->setEnabled(false);
   // QMessageBox::information(this,tr("Blokada głównego okna"),message);
    showLog(message);
    m_blockWindowTimer = new QTimer(this);
    connect(m_blockWindowTimer,SIGNAL(timeout()),this,SLOT(unlockWindow()));
    if(timeMS > 0)
        m_blockWindowTimer->start(timeMS);
}

void MainWindow::showLog(QString caption, QVector<ErrorType> errors)
{
    caption += "<br>";
    int priority = 0;

    while(errors.size()>0)
    {
        switch (errors.front())
        {
        case e00_problem_header:
            caption += QString(tr("- błędny nagłówek <br>"));
            if(priority < MsgPriorityType::errorText) priority = MsgPriorityType::errorText;
            break;
        case e01_problem_params:
            caption += QString(tr("- błędna lista parametrów <br>"));
            if(priority < MsgPriorityType::errorText) priority = MsgPriorityType::errorText;
            break;
        case e02_lost_param:
            caption += QString(tr("- nie udało się odczytać parametru <br>"));
            if(priority < MsgPriorityType::warningText) priority = MsgPriorityType::warningText;
            break;
        case e10_serial_not_opened:
            caption += QString(tr("- nie udało się otworzyć portu COM <br>"));
            if(priority < MsgPriorityType::errorText) priority = MsgPriorityType::errorText;
            break;
        case e11_serial_not_closed:
            caption += QString(tr("- nie udało się zamknąć połączenia COM <br>"));
            if(priority < MsgPriorityType::errorText) priority = MsgPriorityType::errorText;
            break;
        case e12_serial_not_reconfigured:
            caption += QString(tr("- nie udało się zmienić konfiguracji portu <br>"));
            if(priority < MsgPriorityType::warningText) priority = MsgPriorityType::warningText;
            break;
        case e13_serial_not_configured:
            caption += QString(tr("- komunikacja szeregowa jest błędnie skonfigurowana <br>"));
            if(priority < MsgPriorityType::warningText) priority = MsgPriorityType::warningText;
            break;
        }
        errors.pop_front();
    }

    ui->logBrowser->append(MsgPriorityColors[priority]+caption+"</font>");
}

void MainWindow::on_forwardButton_pressed()
{
    m_moveType = MoveType::moveForward;

    m_buttonHoldTimer = new QTimer(this);
    connect(m_buttonHoldTimer,SIGNAL(timeout()),this,SLOT(requestRobotMove()));
    m_buttonHoldTimer->start(100);
}

void MainWindow::on_forwardButton_released()
{
    m_buttonHoldTimer->stop();
    delete m_buttonHoldTimer;
}

void MainWindow::on_rightButton_pressed()
{
    m_moveType = MoveType::moveRight;

    m_buttonHoldTimer = new QTimer(this);
    connect(m_buttonHoldTimer,SIGNAL(timeout()),this,SLOT(requestRobotMove()));
    m_buttonHoldTimer->start(100);
}

void MainWindow::on_rightButton_released()
{
    m_buttonHoldTimer->stop();
    delete m_buttonHoldTimer;
}

void MainWindow::on_leftButton_pressed()
{
    m_moveType = MoveType::moveLeft;

    m_buttonHoldTimer = new QTimer(this);
    connect(m_buttonHoldTimer,SIGNAL(timeout()),this,SLOT(requestRobotMove()));
    m_buttonHoldTimer->start(100);
}

void MainWindow::on_leftButton_released()
{
    m_buttonHoldTimer->stop();
    delete m_buttonHoldTimer;
}

void MainWindow::requestRobotMove()
{
    double forwardStep = 0.025;
    double angleStep = 3.0;
    RobotOrientation orientation = m_robotController.getExpectedOrientation();
    if(m_diffOrientation.angleY>180.0) m_diffOrientation.angleY -= 360.0;
    else if(m_diffOrientation.angleY<-180.0) m_diffOrientation.angleY += 360.0;
    double angleRad = (180.0-m_diffOrientation.angleY-orientation.angleY)*3.14/180.0;
    switch(m_moveType)
    {
    case MoveType::moveForward: m_diffOrientation.position += QVector4D(-sin(angleRad)*forwardStep,0.0f,cos(angleRad)*forwardStep,0.0f); break;
    case MoveType::moveRight: m_diffOrientation.angleY -= angleStep; break;
    case MoveType::moveLeft: m_diffOrientation.angleY += angleStep; break;
    case MoveType::moveBack: m_diffOrientation.position -= QVector4D(-sin(angleRad)*forwardStep,0.0f,cos(angleRad)*forwardStep,0.0f); break;
    default: break;
    }

    QString caption = QString("<font color=\"white\">" +  tr("Przemieszczenie ") +
                                QString(" X:") +QString::number(m_diffOrientation.position.x(),'f',2) +
                                QString(" Y:") + QString::number(m_diffOrientation.position.y(),'f',2) +
                                QString(" Z:") + QString::number(m_diffOrientation.position.z(),'f',2) +
                                tr(" kątY:") + QString::number(m_diffOrientation.angleY,'f',1));
    ui->sterowanieLabel->setText(caption);


    m_monitor->setRequestRobotOrientation(orientation.position+m_diffOrientation.position, orientation.angleY+m_diffOrientation.angleY);
}

void MainWindow::on_clearMoveButton_pressed()
{
    m_diffOrientation.position = QVector4D(0,0,0,0);
    m_diffOrientation.angleY = 0.0f;
    m_moveType = MoveType::stop;
    requestRobotMove();
}

void MainWindow::on_actionO_autorze_triggered()
{
    author.show();
}

void MainWindow::on_actionKonfiguracja_triggered()
{
    comWindow.show();
}

void MainWindow::on_actionConnectSerial_triggered()
{
    if(m_robotController.openSerial())
    {
        //ui->ControlWidget->setEnabled(true);
        ui->gb_scanEnv->setEnabled(true);
        ui->gb_sKrok->setEnabled(true);
        ui->actionKonfiguracja->setEnabled(false);
        ui->actionConnectSerial->setEnabled(false);
        ui->actionDisconnectSerial->setEnabled(true);
        ui->polaczenieLabel->setText("<font color=\"green\"><b>COM: Podłączony</b></font>");
    }
}

void MainWindow::on_actionDisconnectSerial_triggered()
{
    m_robotController.closeSerial();
  //  ui->ControlWidget->setEnabled(false);
    ui->gb_scanEnv->setEnabled(false);
    ui->gb_sKrok->setEnabled(false);
    ui->actionKonfiguracja->setEnabled(true);
    ui->actionConnectSerial->setEnabled(true);
    ui->actionDisconnectSerial->setEnabled(false);
    ui->polaczenieLabel->setText("<font color=\"red\"><b>COM: Rozłączony</b></font>");
}

void MainWindow::on_actionShowSerialInfo_triggered()
{
    QMessageBox::information(this,"Aktualne ustawienia COM", m_robotController.getInfoAboutSerial());

}

void MainWindow::on_pb_moveLeftStepper_clicked()
{
    m_robotController.moveStepperMotor(0,ui->sb_angle->value(),ui->sb_vSpeedCallibration->value());
}

void MainWindow::on_pb_moveRightStepper_clicked()
{
    m_robotController.moveStepperMotor(1,ui->sb_angle->value(),ui->sb_vSpeedCallibration->value());
}

void MainWindow::on_pb_scanEnv_clicked()
{
    QVector<QVector<QVector4D>*> * verts = m_robotController.scanTerritory(ui->sb_cMeasurments->value(),ui->sb_vSpeedScan->value());
    if(verts==nullptr)
        QMessageBox::critical(this,tr("Błąd odczytu mapy"),tr("Nie udało się zinterpretować wiadomości i narysować mapy"));
    else
    {
        RobotOrientation orientation = m_robotController.getExpectedOrientation();
        m_monitor->addEnvMap(verts,orientation.position,orientation.angleY);
        delete verts;
    }
}

void MainWindow::on_pb_moveRobot_clicked()
{
    m_robotController.moveRobot(m_diffOrientation);
    m_diffOrientation.angleY = 0.0f;
    m_diffOrientation.position = QVector4D(0.0f,0.0f,0.0f,0.0f);
}

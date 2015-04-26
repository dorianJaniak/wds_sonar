#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

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
    m_buttonHoldTimer(nullptr)
{
    ui->setupUi(this);
    m_monitor = ui->mainView;

    connect(&m_fileController,SIGNAL(sendLog(QString,QVector<ErrorType>)),this,SLOT(showLog(QString,QVector<ErrorType>)));

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
        m_monitor->addEnvMap(verts,m_robotController.getExpectedOrientation().position);
        delete verts;
    }
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
    if(m_diffOrientation.angleY>180.0) m_diffOrientation.angleY -= 360.0;
    else if(m_diffOrientation.angleY<-180.0) m_diffOrientation.angleY += 360.0;
    double angleRad = (180.0-m_diffOrientation.angleY)*3.14/180.0;
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

    RobotOrientation orientation = m_robotController.getActualOrientation();
    m_monitor->setRequestRobotOrientation(orientation.position+m_diffOrientation.position, orientation.angleY+m_diffOrientation.angleY);
}

void MainWindow::on_clearMoveButton_pressed()
{
    m_diffOrientation.position = QVector4D(0,0,0,0);
    m_diffOrientation.angleY = 0.0f;
    m_moveType = MoveType::stop;
    requestRobotMove();
}

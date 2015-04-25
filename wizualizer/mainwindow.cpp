#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    m_fileController(parent)
{
    ui->setupUi(this);
    m_monitor = ui->mainView;

    connect(&m_fileController,SIGNAL(sendLog(QString,QVector<ErrorType>)),this,SLOT(showLog(QString,QVector<ErrorType>)));
}

MainWindow::~MainWindow()
{
    delete ui;
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
        m_monitor->addEnvMap(verts,QVector4D(0.0f,0.0f,0.0f,1.0f));
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

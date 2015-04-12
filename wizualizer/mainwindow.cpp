#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filecontroller.h"

#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _monitor = ui->mainView;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoadFromSimFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Wczytaj plik symulacyjny"),".",tr("Plik symulacyjny (*)"));
    FileController::getFromCSVFile(fileName);
 //   if()
}

#include "comwindow.h"
#include "ui_comwindow.h"

#include <QtSerialPort/QSerialPort>
#include <QAbstractButton>
#include <QDebug>

ComWindow::ComWindow(QWidget *parent) :
    QDialog(parent),
    QSerialPortInfo(),
    ui(new Ui::ComWindow)
{
    ui->setupUi(this);

    fill();
}

ComWindow::~ComWindow()
{
    delete ui;
}

void ComWindow::fill()
{
    serials = availablePorts();
    QList<qint32> bauds = standardBaudRates();

    for(unsigned i=0; i<serials.size(); i++)
        ui->cb_port->addItem(serials[i].portName(), QVariant(i));

    for(unsigned i=0; i<bauds.size();i++)
        ui->cb_baud->addItem(QString::number(bauds.at(i)),QVariant(bauds.at(i)));

    //Poniższe instrukcje się nie skompilują jeśli nie będą istnieć
    //(sprawdzam w ten sposób zgodność z wersją biblioteki)
   // QSerialPort::Data7; QSerialPort::Data8;
    ui->cb_bits->addItem(QString::number(8),QSerialPort::Data8);
    ui->cb_bits->addItem(QString::number(7),QSerialPort::Data7);

    ui->cb_flowcontrol->addItem("None",QSerialPort::NoFlowControl);
    ui->cb_flowcontrol->addItem("RTS/CTS",QSerialPort::HardwareControl);
    ui->cb_flowcontrol->addItem("XON/XOFF",QSerialPort::SoftwareControl);

    ui->cb_parity->addItem("None",QSerialPort::NoParity);
    ui->cb_parity->addItem("Even",QSerialPort::EvenParity);
    ui->cb_parity->addItem("Odd", QSerialPort::OddParity);
    ui->cb_parity->addItem("Space",QSerialPort::SpaceParity);
    ui->cb_parity->addItem("Mark", QSerialPort::MarkParity);

    ui->cb_stopbit->addItem(QString::number(1),QSerialPort::OneStop);
    ui->cb_stopbit->addItem(QString::number(2),QSerialPort::TwoStop);
}

void ComWindow::on_pb_refresh_clicked()
{
    serials.clear();
    ui->cb_port->clear();
    serials = availablePorts();
    for(unsigned i=0; i<serials.size(); i++)
        ui->cb_port->addItem(serials[i].portName(), QVariant(i));
}

void ComWindow::on_buttonBox_clicked(QAbstractButton * button)
{
    if((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        emit updateSerial(serials.at(ui->cb_port->currentData().toInt()),
                          (QSerialPort::BaudRate)ui->cb_baud->currentData().toInt(),
                          (QSerialPort::DataBits)ui->cb_bits->currentData().toInt(),
                          (QSerialPort::FlowControl)ui->cb_flowcontrol->currentData().toInt(),
                          (QSerialPort::Parity)ui->cb_parity->currentData().toInt(),
                          (QSerialPort::StopBits)ui->cb_stopbit->currentData().toInt());
    }
    done(1);
}

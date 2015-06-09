#ifndef COMWINDOW_H
#define COMWINDOW_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>

class QAbstractButton;

namespace Ui {
class ComWindow;
}

class ComWindow : public QDialog, QSerialPortInfo
{
    Q_OBJECT
    QList<QSerialPortInfo> serials;
public:
    explicit ComWindow(QWidget *parent = 0);
    ~ComWindow();

private slots:
    void on_pb_refresh_clicked();
    void on_buttonBox_clicked(QAbstractButton * button);

signals:
    void updateSerial(const QSerialPortInfo& port, QSerialPort::BaudRate br, QSerialPort::DataBits db, QSerialPort::FlowControl fc, QSerialPort::Parity p, QSerialPort::StopBits sb);

private:
    void fill();

    Ui::ComWindow *ui;

};

#endif // COMWINDOW_H

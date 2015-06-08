#ifndef COMWINDOW_H
#define COMWINDOW_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class ComWindow;
}

class ComWindow : public QDialog, QSerialPortInfo
{
    Q_OBJECT

public:
    explicit ComWindow(QWidget *parent = 0);
    ~ComWindow();

private slots:
    void on_pb_refresh_clicked();

private:
    void fill();

    Ui::ComWindow *ui;

};

#endif // COMWINDOW_H

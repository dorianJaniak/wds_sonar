#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class OpenGLWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    OpenGLWindow *_monitor;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapviewer.h"
#include "filecontroller.h"

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
    MapViewer *_monitor;
    FileController _fileController;

private slots:
    void on_actionLoadFromSimFile_triggered();

    void showLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());
};

#endif // MAINWINDOW_H

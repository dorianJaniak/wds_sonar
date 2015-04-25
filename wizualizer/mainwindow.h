#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "mapviewer.h"
#include "filecontroller.h"
#include "robotcontroller.h"
#include "globalvariables.h"

class OpenGLWindow;

namespace Ui {
class MainWindow;
}

/**
 * @brief Główne okno aplikacji
 * Klasa odpowiada rysowanemu głównemu oknu aplikacji. Przechowuje kontrolki,
 * i widżety.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MapViewer * m_monitor;                  ///Wskaźnik na widżet obsługujący widok 3D.
    FileController m_fileController;        ///Obsługa plików dysków. Umożliwia wczytywanie danych.
    RobotController m_robotController;      ///Obsługa robota poprzez interfejs Bluetooth.

    QTimer m_buttonHoldTimer;               ///Odliczanie czasu trzymania przycisku
    RobotOrientation m_diffOrientation;     ///Przekazywana do aktualizowania widoku zmienna
    MoveType m_moveType;                    ///Zliczany aktualnie ruch
private slots:
                        /**
     * @brief Menu-Plik-Załaduj z pliku symulacyjnego
     * Reakcja na rządanie załadowania pliku symulacyjnego.
     */
    void on_actionLoadFromSimFile_triggered();

                        /**
     * @brief Funkcja dodająca logi o opisie caption oraz opcjonalnej liście błędów errors.
     * @param caption Opis komunikatu
     * @param errors Wektor błędów
     */
    void showLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());
    void on_forwardButton_pressed();
    void on_forwardButton_released();
    void requestRobotMove();
    void on_rightButton_pressed();
    void on_rightButton_released();
    void on_leftButton_pressed();
    void on_leftButton_released();
};

#endif // MAINWINDOW_H

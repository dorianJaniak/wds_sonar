#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "mapviewer.h"
#include "filecontroller.h"
#include "robotcontroller.h"
#include "globalvariables.h"
#include "author.h"
#include "comwindow.h"

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

    void initialize();

    Ui::MainWindow *ui;
    Author author;
    ComWindow comWindow;

    MapViewer * m_monitor;                  ///<Wskaźnik na widżet obsługujący widok 3D.
    FileController m_fileController;        ///<Obsługa plików dysków. Umożliwia wczytywanie danych.
    RobotController m_robotController;      ///<Obsługa robota poprzez interfejs Bluetooth.

    QTimer * m_blockWindowTimer;            ///<Odlicza czas blokowania głównego okna
    QTimer * m_buttonHoldTimer;               ///<Odliczanie czasu trzymania przycisku
    RobotOrientation m_diffOrientation;     ///<Przekazywana do aktualizowania widoku zmienna
    MoveType m_moveType;                    ///<Zliczany aktualnie ruch

private slots:
                        /**
     * @brief Menu-Plik-Załaduj z pliku symulacyjnego
     * Reakcja na rządanie załadowania pliku symulacyjnego.
     */
    void on_actionLoadFromSimFile_triggered();

    void unlockWindow();
    void blockWindow(QString message, unsigned timeMS);

                        /**
     * @brief Funkcja dodająca logi o opisie caption oraz opcjonalnej liście błędów errors.
     * @param caption Opis komunikatu
     * @param errors Wektor błędów
     */
    void showLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());

                        /**
     * @brief Reakcja na naciśnięcie przycisku "w przód"
     *
     * Uruchamia timer, który odlicza czas przez który naciśnięty jest przycisk.
     */
    void on_forwardButton_pressed();
                        /**
     * @brief Reakcja na puszczenie przycisku "w przód"
     *
     * Stopuje wcześniej uruchomiony timer i go usuwa.
     */
    void on_forwardButton_released();
                        /**
     * @brief Przygotowania żądania ruchu robota w oknie 3D
     *
     * Funkcja wylicza na podstawie zmiennej m_diffOrientation względną
     * zmianę położenia i orientacji robota, a następnie wywołuję funkcję
     * zmieniającą położenia robota z okna 3D w klasie m_monitor
     */
    void requestRobotMove();
                        /**
    * @brief Reakcja na naciśnięcie przycisku "w prawo"
    *
    * Uruchamia timer, który odlicza czas przez który naciśnięty jest przycisk.
    */
    void on_rightButton_pressed();
                        /**
    * @brief Reakcja na puszczenie przycisku "w lewo"
    *
    * Stopuje wcześniej uruchomiony timer i go usuwa.
    */
    void on_rightButton_released();
                        /**
    * @brief Reakcja na naciśnięcie przycisku "w lewo"
    *
    * Uruchamia timer, który odlicza czas przez który naciśnięty jest przycisk.
    */
    void on_leftButton_pressed();
                        /**
    * @brief Reakcja na puszczenie przycisku "w lewo"
    *
    * Stopuje wcześniej uruchomiony timer i go usuwa.
    */
    void on_leftButton_released();
                        /**
     * @brief Reakcja na naciśnięcie przycisku "Powrót"
     *
     * Zeruje pozycję robota.
     */
    void on_clearMoveButton_pressed();
    void on_actionO_autorze_triggered();
    void on_actionKonfiguracja_triggered();
    void on_actionConnectSerial_triggered();

    void on_actionDisconnectSerial_triggered();
    void on_actionShowSerialInfo_triggered();

    void on_pb_moveLeftStepper_clicked();
    void on_pb_moveRightStepper_clicked();
    void on_pb_scanEnv_clicked();
};

#endif // MAINWINDOW_H

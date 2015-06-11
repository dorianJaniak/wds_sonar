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

    /**
     * @brief Funkcja inicjalizująca
     *
     * Funkcja inicjalizuje i zeruje składowe klasy oraz od razu
     * ładuje obiekt reprezentujący robota w oknie 3D.
     */
    void initialize();

    Ui::MainWindow *ui;
    Author author;                          ///<Obiekt związany z widokiem okna informacji o autorze
    ComWindow comWindow;                    ///<Obiekt związany z widokiem okna konfiguracji połączeń szeregowych

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

                        /**
     * @brief Funkcja odblokowująca okno
     *
     * Funkcja odblokowuje widżet Kontrola oraz pasek menu.
     * Nie należy jej wywoływać bezpośrednio, ponieważ usuwa ona
     * obiekt wskazywany przez wskaźnik m_blockWindowTimer.
     */
    void unlockWindow();
                        /**
     * @brief Funkcja blokująca widok
     *
     * Funkcja blokuje widżet Kontrola i pasek menu na czas podany przez timeMS oraz
     * publikuje komunikat message w oknie logów.
     * @param[in] message komunikat
     * @param[in] timeMS czas blokowania w milisekundach
     */
    void blockWindow(QString message, unsigned timeMS);

                        /**
     * @brief Funkcja dodająca logi o opisie caption oraz opcjonalnej liście błędów errors.
     * @param[in] caption Opis komunikatu
     * @param[in] errors Wektor błędów
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
                        /**
     * @brief Reakcja na akcję w menu - o autorze.
     *
     * Pokazuje okno Authors prezentujące informacje o autorze.
     */
    void on_actionO_autorze_triggered();
                        /**
     * @brief Reakcja na akcję w menu - konfiguracja
     *
     * Funkcja pokazuje okno ComWindow związane z konfiguracją połączenia szeregowego.
     */
    void on_actionKonfiguracja_triggered();
                        /**
     * @brief Reakcja na akcję w menu - Połącz
     *
     * Próbuje otworzyć połączenie szeregowe i jeśli to się udaje
     * to odsłania wszystkie pola widżetu Kontrola oraz deaktywuje akcję
     * Połącz i aktywuje akcję Rozłącz.
     */
    void on_actionConnectSerial_triggered();
                        /**
     * @brief Reakcja na akcję w menu - rozłącz
     *
     * Funkcja rozłącza połączenie szeregowe oraz deaktywuje część opcji
     * widżetu Kontrola i akcję w menu Rozłącz, natomiast aktywuje
     * akcję Połącz.
     */
    void on_actionDisconnectSerial_triggered();
                        /**
     * @brief Reakcja na akcję w menu - Wyświetl informacje
     *
     * Funkcja pokazuje jedynie okno MessageBox z zestawieniem informacji
     * o aktualnie ustawionej konfiguracji połączenia szeregowego.
     */
    void on_actionShowSerialInfo_triggered();

                        /**
     * @brief Funkcja kalibrująca.
     *
     * Funkcja pozwala na skalibrowanie silnika krokowego.
     * Obraca silnik zgodnie z zasadą prawej dłoni.
     * Wcześniej pobiera informacje z formularza widżetu Kontrola.
     */
    void on_pb_moveLeftStepper_clicked();
                        /**
    * @brief Funkcja kalibrująca.
    *
    * Funkcja pozwala na skalibrowanie silnika krokowego.
    * Obraca silnik w przeciwnym kierunku niż w zasadzie prawej dłoni.
    * Wcześniej pobiera informacje z formularza widżetu Kontrola.
    */
    void on_pb_moveRightStepper_clicked();
                        /**
     * @brief Funkcja wywołująca skanowanie terenu.
     *
     * Funkcja pobiera ustawione informacje w formularzu Kontrola, a
     * następnie przekazuje je do funkcji scanTerritory z obiektu klasy
     * RobotController. Jeśli operacja się nie powiedzie to wyświetli
     * odpowiedni komunikat.
     */
    void on_pb_scanEnv_clicked();
                        /**
     * @brief Funkcja żądająca przemieszczenia robota.
     *
     * Funkcja pobiera informacje z formularza widżetu Kontrola, a
     * następnie wywołuje funkcję moveRobot, która ma przemieścić robota.
     * Niezależnie od powodzenia operacji zeruje wektor przemieszczenia
     * m_diffOrientation.
     */
    void on_pb_moveRobot_clicked();
};

#endif // MAINWINDOW_H

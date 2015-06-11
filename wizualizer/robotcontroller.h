#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QVector4D>
#include "messagecontroller.h"
#include "bluetoothcontroller.h"
#include "globalvariables.h"
#include <QtSerialPort/QSerialPort>
#include <QObject>

/**
 * @brief Struktura RobotOrientation
 *
 * Struktura przechowuje informacje o pozycji robota
 */
struct RobotOrientation {
    QVector4D position;         ///<Położenie robota na mapie w stabilnych współrzędnych X,Y,Z,W
    float angleY;               ///<Obrót wokół własnej osi robota
};

/**
 * @brief Klasa obsługująca robota
 *
 * Klasa może obsługiwać robota. Zawiera informacje o jego położeniu,
 * umie się z nim łączyć oraz umie obsługiwać komunikaty przez niego wysyłane.
 */
class RobotController : public MessageController
{
    Q_OBJECT
    QSerialPort serial;         ///<Obiekt obsługujący połączenie szeregowe
                        ///Wysterowana realnie pozycja (na podstawie enkoderów i algorytmu)
    RobotOrientation m_robotActualOrient;
                         ///Żądana pozycja dla robota (zatwierdzona w programie)
    RobotOrientation m_robotExpectedOrient;
public:
                        /**
     * @brief Konstruktor
     *
     * Zeruje jedynie zmienne.
     */
    RobotController();
    ~RobotController();

                        /**
     * @brief Pobiera prawdziwą aktualną pozycję robota
     * @return aktualna pozycja robota we współrzędnych jednorodnych
     */
    RobotOrientation getActualOrientation();
                        /**
    * @brief Pobiera żądaną pozycję robota
    * @return oczekiwana pozycja robota we współrzędnych jednorodnych
    */
    RobotOrientation getExpectedOrientation();
                        /**
     * @brief Obsługa żądania przemieszczenia robota
     *
     * Wywołanie funkcji wiąże się z wysłaniem odpowiednich ramek do robota, a
     * więc też z jego przemieszczeniem. Robot będzie próbował się przemieścić
     * do zadanej pozycji. Przy okazji dane zostaną zapisane w zmiennej m_robotExpectedOrient.
     * Funkcja po przesłaniu wiadomości do robota zablokuje odpowiednie okna poprzez wysłanie sygnału blockWindow,
     * a następnie rozpocznie oczekiwanie na odpowiedź. Jeśli odpowiedź okaże się poprawna to wartości otrzymane z
     * odpowiedzi zostaną dodane do ostatnio zapamiętanej pozycji robota.
     * @param[in] displacement Przemieszczenie robota we współrzędnych mapy.
     * @param[in] vCMperSspeed prędkość przemieszczenia robota w centymetrach/sekundę.
     * @param[in] toSerial true jeśli wiadomość ma zostać przesłana poprzez port szeregowy, false
     * jeśli ma zostać obsłużona jedynie wewnątrz programu.
     * @return true jeśli operacja wykonała się poprawnie, false w przeciwnym wypadku.
     */
    bool moveRobot(RobotOrientation displacement, unsigned vCMperSspeed, bool toSerial = false);

                        /**
     * @brief Obsługa żądania zmiany obrotu silnika krokowego.
     *
     * Wywołanie funkcji wiąże się z wysłaniem wiadomości o obrocie silnika. Funkcja służy
     * do kalibracji kąta akustycznego sonaru. Po wysłaniu wiadomości zostanie zablokowany
     * interfejs użytkownika poprzez wysłanie sygnału blockWindow. Następnie funkcja będzie oczekiwać
     * na wiadomość zwrotną od urządzenia.
     * @param[in] directionRight false jeśli zgodnie z zasadą prawej dłoni, w przeciwnym wypadku false.
     * @param[in] angle kąt obrotu wyrażony w stopniach
     * @param[in] vDegreeSpeed szybkość obrotu wyrażona w stopniac/sekundę.
     * @return true jeśli operacja się powiodła, w przeciwnym wypadku false.
     */
    bool moveStepperMotor(bool directionRight, unsigned angle, unsigned vDegreeSpeed);
                        /**
     * @brief Obsługa żądania wykonania skanowania otoczenia.
     *
     * Wywołanie funkcji wiąże się z wysłaniem wiadomości W02. Funkcja służy do
     * wykonania skanu otoczenia oraz odebrania rezultatów skanu. Po wysłaniu wiadomości zostanie zablokowany
     * interfejs użytkownika poprzez wysłanie sygnału blockWindow. Następnie funkcja będzie oczekiwać
     * na wiadomość zwrotną od urządzenia.
     * @param[in] cMeasurements ilość pomiarów, które mają zostać wykonane w zakresie 180 stopni.
     * @param[in] vDegreeSpeed szybkość obrotu wyrażona w stopniach na sekundę
     * @return nullptr gdy wystąpił błąd krytyczny dla operacji odczytu danych,
     * w przeciwnym wypadku wskaźnik na pełny pakiet wierzchołków potrzebny do
     * stworzenia obiektu EnvMap.
     */
    QVector<QVector<QVector4D>*> * scanTerritory(unsigned cMeasurements ,unsigned vDegreeSpeed);

                        /**
     * @brief Otwarcie połączenia szeregowego.
     *
     * Otwiera połączenie szeregowe, sprawdzając czy nie nastąpiły błędy.
     * @return true jeśli otwarto połączenie, false w przeciwnym wypadku.
     */
    bool openSerial();
                        /**
     * @brief Zamknięcie połączenia szeregowego.
     * @return zawsze true.
     */
    bool closeSerial();
                        /**
     * @brief Pobranie aktualnych ustawień portu szeregowego.
     * @return tekst z pełną informacją nt. ustawień portu szeregowego.
     */
    QString getInfoAboutSerial();

signals:
        /**
    * @brief Sygnał blokowania widoku.
    * @param[in] message komunikat
    * @param[in] timeMS czas blokowania w milisekundach
    */
    void blockWindow(QString message, unsigned timeMS);

public slots:
                        /**
     * @brief Slot obsługujący żądanie zmiany parametrów połączenia szeregowego.
     * @param[in] port - obiekt klasy QSerialPortInfo
     * @param[in] br - szybkość bodów
     * @param[in] db - ilość bitów
     * @param[in] fc - kontrola przepływu
     * @param[in] p - parzystość
     * @param[in] sb - ilość bitów stopu
     */
    void reconfigureSerial(const QSerialPortInfo& port, QSerialPort::BaudRate br, QSerialPort::DataBits db, QSerialPort::FlowControl fc, QSerialPort::Parity p, QSerialPort::StopBits sb);

private:
                        /**
     * @brief Funkcja ustawiająca najczęściej wybieraną konfigurację połączenia COM.
     */
    void initializeSerial();
                        /**
     * @brief Czyszczenie buforów wejściowych oraz flag błędów.
     */
    void clearSerialBuffers();
                        /**
     * @brief Funkcja oczekująca na zestaw danych z portu szeregowego
     *
     * Funkcja oczekuje na dane wejściowe z portu szeregowego przez czas 3*timeMS.
     * Stara się odebrać całą wiadomość (do znaku LF).
     * @param timeMS czas oczekiwania w milisekundach.
     * @return odczytana wiadomość.
     */
    QString readMessageFromSerial(unsigned timeMS);
};

#endif // ROBOTCONTROLLER_H

#ifndef COMWINDOW_H
#define COMWINDOW_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>

class QAbstractButton;

namespace Ui {
class ComWindow;
}

/*!
 * \brief Okno konfiguracji połączenia COM
 *
 * Klasa obsługuje interfejs graficzny związany z konfiguracją połączenia szeregowego.
 * Uzupełnia modele o informacje dostępne z QSerialPortInfo.
 */
class ComWindow : public QDialog, QSerialPortInfo
{
    Q_OBJECT
    QList<QSerialPortInfo> serials;             ///< lista wszystkich dostępnych portów szeregowych
public:
    /*!
     * \brief Konstruktor ComWindow
     *
     * Jedynie inicjalizuje UI oraz wywołuje funkcję fill.
     * \param[in] parent
     */
    explicit ComWindow(QWidget *parent = 0);
    ~ComWindow();

private slots:
    /*!
     * \brief Funkcja odświeżająca widok.
     *
     * Funkcja pozwala na odświeżenie listy dostępnych portów szeregowych (dokładnie ich nazw).
     */
    void on_pb_refresh_clicked();
    /*!
     * \brief Funkcja kliknięcia na standardowe przyciski
     *
     * Funkcja obsługuje przycisk Apply. Emituje sygnał żądający zmiany konfiguracji wybranego portu szeregowego na podstawie
     * uzupełnionych pól w oknie ComWindow.
     * \param[in] button - przycisk naciśnięty.
     */
    void on_buttonBox_clicked(QAbstractButton * button);

signals:
    /*!
     * \brief Sygnał żądania zmiany konfiguracji portu szeregowego
     * \param[in] port - obiekt klasy QSerialPortInfo
     * \param[in] br - szybkość bodów
     * \param[in] db - ilość bitów
     * \param[in] fc - kontrola przepływu
     * \param[in] p - parzystość
     * \param[in] sb - ilość bitów stopu
     */
    void updateSerial(const QSerialPortInfo& port, QSerialPort::BaudRate br, QSerialPort::DataBits db, QSerialPort::FlowControl fc, QSerialPort::Parity p, QSerialPort::StopBits sb);

private:
    /*!
     * \brief Funkcja uzupełniająca modele.
     *
     * Funkcja pobiera informacje z QSerialPortInfo nt. dostępnych portów szeregowych oraz
     * wybrane opcje konfiguracji z QSerialPort, a następnie uzupełnia o nie modele stowarzyszone
     * z combobox'ami w oknie ComWindow. Powinna zostać wywołana tylko raz - przy utworzeniu obiektu
     * klasy.
     */
    void fill();

    Ui::ComWindow *ui;

};

#endif // COMWINDOW_H

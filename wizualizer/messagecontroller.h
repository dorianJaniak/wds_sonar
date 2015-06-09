#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QString>
#include <QVector>
#include <QObject>


class QVector4D;

enum MessageTypes {
    w_ok = 0,
    w_start,
    w_scan,
    w_map_data,
    w_calibrate,
    w_move,
    w_resultMove,
    w_error,
    w_none
};

enum ErrorType {
    e00_problem_header = 0,
    e01_problem_params,
    e02_lost_param,

    e10_serial_not_opened = 10,
    e11_serial_not_closed = 11,
    e12_serial_not_reconfigured = 12,
    e13_serial_not_configured = 13,
    e_no_error
};

/**
 * @brief Klasa interpretuje i przygotowuje wiadomości.
 * Wiadomości są potrzebne w komunikacji z urządzeniem (robotem) oraz przy
 * odczytywaniu plików symulacyjnych z pliku.
 */
class MessageController : public QObject
{
    Q_OBJECT
    static QString m_wNames[];              ///<Nagłówki wiadomości
public:
    MessageController();
    ~MessageController();

    MessageTypes checkMessageType(const QString & msg);
    QString prepareStringFromList(QStringList list);
    QStringList prepareListFromString(QString msg);

    bool reinterpretW00(QStringList allFields);
    bool reinterpretW01(QStringList & allFields);

    QStringList prepareW02(unsigned cMeasurements, unsigned vDegreeSpeed);

                    /**
     * @brief Interpretuje wiadomość W03. Sprawdza jej poprawność i przygotowuje na jej podstawie pełny pakiet wierzchołków.
     * @param allFields Wydobyte pola wiadomości W03.
     * @return nullptr jeśli nie uda się zinterpretować wiadomości, w przeciwnym wypadku pełny pakiet wierzchołków
     */
    QVector<QVector<QVector4D>*> * reinterpretW03(QStringList & allFields);

    QStringList prepareW04(bool directionRight, unsigned angle, unsigned vDegreeSpeed);

signals:
                    /**
     * @brief Sygnał raportujący wektor błędów lub komunikat.
     * @param caption Treść komunikatu.
     * @param errors Wektor błędów.
     */
    void sendLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());

};

#endif // MESSAGECONTROLLER_H

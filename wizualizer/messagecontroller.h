#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QString>
#include <QVector>
#include <QObject>


class QVector4D;

enum MessageTypes {
    w00_map_data = 0
};

enum ErrorType {
    e00_problem_header = 0,
    e01_problem_params,
    e02_lost_param,

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
    static QString m_wNames[];              ///Nagłówki wiadomości
public:
    MessageController();
    ~MessageController();

                    /**
     * @brief Interpretuje wiadomość W00. Sprawdza jej poprawność i przygotowuje na jej podstawie pełny pakiet wierzchołków.
     * @param allFields Wydobyte pola wiadomości W00.
     * @return nullptr jeśli nie uda się zinterpretować wiadomości, w przeciwnym wypadku pełny pakiet wierzchołków
     */
    QVector<QVector<QVector4D>*> * reinterpretW00(QStringList & allFields);

signals:
                    /**
     * @brief Sygnał raportujący wektor błędów lub komunikat.
     * @param caption Treść komunikatu.
     * @param errors Wektor błędów.
     */
    void sendLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());

};

#endif // MESSAGECONTROLLER_H

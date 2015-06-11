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

                    /**
     * @brief Funkcja sprawdzająca typ wiadomości
     * @param[in] msg nagłówek wiadomości
     * @return typ wiadomości MessageTypes lub w_none gdy nie znany nagłówek
     */
    MessageTypes checkMessageType(const QString & msg);
                    /**
     * @brief Funkcja przygotowująca wiadomość w formacie CSV z pól wiadomości.
     * @param[in] list lista pól wiadomości
     * @return Wiadomość w formacie CSV oddzielona średnikami.
     */
    QString prepareStringFromList(QStringList list);
                    /**
     * @brief Funkcja przygotowująca listę pól z wiadomości w formacie CSV.
     * @param[in] msg wiadomość w formacie CSV oddzielonym średnikami.
     * @return lista wszystkich pól wiadomości
     */
    QStringList prepareListFromString(QString msg);

                    /**
     * @brief Funkcja interpretująca wiadomość OK.
     * @param[in] allFields lista wszystkich pól wiadomości OK.
     * @return true gdy format wiadomości poprawny, false w przeciwnym wypadku.
     */
    bool reinterpretW00(QStringList allFields);
                    /**
    * @brief Funkcja interpretująca wiadomość START.
    * @param[in] allFields lista wszystkich pól wiadomości START.
    * @return true gdy format wiadomości poprawny, false w przeciwnym wypadku.
    */
    bool reinterpretW01(QStringList & allFields);

                    /**
     * @brief Funkcja przygotowująca wiadomość żądającą wykonania skanowania otoczenia.
     * @param[in] cMeasurements ilość pomiarów jakie mają zostać wykonane w zakresie 180 stopni obrotu.
     * @param[in] vDegreeSpeed prędkość obrotu wyrażona w jednostce stopień/sekundę.
     * @return lista wszystkich pól wiadomości W02.
     */
    QStringList prepareW02(unsigned cMeasurements, unsigned vDegreeSpeed);

                    /**
     * @brief Interpretuje wiadomość W03. Sprawdza jej poprawność i przygotowuje na jej podstawie pełny pakiet wierzchołków.
     * @param allFields Wydobyte pola wiadomości W03.
     * @return nullptr jeśli nie uda się zinterpretować wiadomości, w przeciwnym wypadku pełny pakiet wierzchołków
     */
    QVector<QVector<QVector4D>*> * reinterpretW03(QStringList & allFields);

                    /**
     * @brief Funkcja przygotowująca wiadomość żądającą obrotu silnika krokowego.
     * @param[in] directionRight false - ruch zgodnie z zasadą prawej dłoni, true - ruch w przeciwnym kierunku
     * @param[in] angle kąt obrotu wyrażony w stopniach
     * @param[in] vDegreeSpeed szybkość obrotu wyrażona w stopniach/sekundę.
     * @return lista wszystkich pól wiadomośći W04.
     */
    QStringList prepareW04(bool directionRight, unsigned angle, unsigned vDegreeSpeed);

                    /**
     * @brief Funkcja przygotowująca wiadomość żądającą przemiesczenia robota.
     * @param[in] vCMperSspeed prędkość robota wyrażona w cm/sekundę.
     * @param[in] relDistCM dystans wyrażony w cm jaki robot ma przejechać po linii prostej
     * @param[in] relRotBM względny obrót jaki robot ma wykonać przed przejechaniem po linii prostej.
     * @param[in] relRotAM względny obrót jaki robot ma wykonać po przejechaniu po linii prostej.
     * @return lista wszystkich pól wiadomości W05.
     */
    QStringList prepareW05(unsigned vCMperSspeed, unsigned relDistCM, int relRotBM, int relRotAM);

                    /**
     * @brief Funkcja interpretująca wynik przemieszczenia robota.
     * @param allFields lista wszystkich pól wiadomości W06.
     * @return wektor 3 elementów [wykonany_obrót_przed_jazdą_po_prostej, wykonany_obrót_po, przejechany_dystans]
     * w przypadku gdy udało się sparsować wiadomość, w przeciwnym wypadku pusty wektor.
     */
    QVector<int> reinterpretW06(QStringList allFields);

signals:
                    /**
     * @brief Sygnał raportujący wektor błędów lub komunikat.
     * @param caption Treść komunikatu.
     * @param errors Wektor błędów.
     */
    void sendLog(QString caption, QVector<ErrorType> errors = QVector<ErrorType>());

};

#endif // MESSAGECONTROLLER_H

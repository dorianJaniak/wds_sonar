#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QString>
#include "messagecontroller.h"

class QVector4D;

/**
 * @brief Obsługa plików dyskowych
 *
 * Klasa udostępnia zestaw funkcji, pozwalających na ładowanie danych z plików dyskowych.
 * Dziedziczy po MessageController, aby móc od razu konwertować i interpretować odczytane dane.
 */
class FileController : public MessageController
{
    Q_OBJECT
    QObject * m_parent;             ///< Rodzic
public:
    FileController(QObject * parent = 0);
    ~FileController();

                        /**
     * @brief Ładuje plik symulacyjny o nazwie fileName
     *
     * @param fileName nazwa pliku
     * @return nullptr gdy wystąpił błąd krytyczny dla operacji odczytu danych,
     * w przeciwnym wypadku wskaźnik na pełny pakiet wierzchołków potrzebny do
     * stworzenia obiektu EnvMap.
     */
    QVector<QVector<QVector4D> *> *loadSensorDataFromFile(const QString & fileName);
                        /**
     * @brief Ładuje plik CSV i parsuje go względem znaku średnika (';')
     *
     * @param fileName nazwa pliku csv
     * @return zawartość kolejnych pól odczytanych z pliku
     */
    QStringList getFromCSVFile(const QString & fileName);
    /**
     * @brief Ładuje obiekt 3D z pliku STL
     *
     * Przyjmuje, że obiekt jest lity (nie ma przerw w siatce).
     * @param fileName nazwa pliku
     * @return Wektor wierzchołków lub nullptr gdy operacja się nie powiedzie.
     */
    QVector<QVector4D>* getFromSTLFile(const QString & fileName);
};

#endif // FILECONTROLLER_H

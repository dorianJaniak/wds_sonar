#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapviewer.h"
#include "filecontroller.h"

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
    /**
     * @brief Wskaźnik na widżet obsługujący widok 3D.
     */
    MapViewer * m_monitor;
    /**
     * @brief Obsługa plików dyskowych.
     * Umożliwia wczytywanie danych.
     */
    FileController m_fileController;

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
};

#endif // MAINWINDOW_H

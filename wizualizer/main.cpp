#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

/**
 * @mainpage Wizualizator danych sonaru
 *
 * Aplikacja powstała w ramach projektu z kursu Wizualizacji Danych Sensorycznych, prowadzonych przez dr inż. Bogdana Kreczmera.
 * Autorem aplikacji jest Dorian Janiak - student kierunku Automatyka i Robotyka na wydziale Elektroniki Politechniki Wrocławskiej.
 * Zadaniem aplikacji jest wysyłanie żądań do robota oraz interpretowania odpowiedzi od urządzenia, a następnie rysowanie mapy otoczenia
 * na podstawie otrzymanych odpowiedzi.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3,3);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

    return a.exec();
}

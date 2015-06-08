#include "author.h"
#include "ui_author.h"

Author::Author(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Author)
{
    ui->setupUi(this);
    ui->label->setText(tr("<html><head/><body><font style=\" font-weight:600;\">O autorze:"
                          "</font><br>Hej, jestem Dorian Janiak, studiuję na Politechnice Wrocławskiej "
                          "na Wydziale Elektroniki, kierunku: Automatyka i Robotyka i specjalności: Robotyka... "
                          "uff, dużo tego do wypisania było.<br><br><font style=\" font-weight:600;\">"
                          "O programie:</font><br>Program powstał w ramach projektu z kursu: wizualizacje danych sensorycznych,"
                          "<br>prowadzonego przez: dr inż. Bogdana Kreczmera<br>Zadaniem programu jest wizualizacja danych z "
                          "sonaru zamieszczonego na robocie mobilnym. Program powstawał w semestrze letnim roku akademickiego "
                          "2015/2016<br><br><font style=\" font-weight:600;\">O robocie:</font><br>"
                          "Robot powstaje w ramach kursu: roboty mobilne. Tworzony jest przez: Doriana Janiaka "
                          "oraz Marcina Ochmana.</body></html>"));
}

void Author::showEvent(QShowEvent *)
{
    adjustSize();
}

Author::~Author()
{
    delete ui;
}

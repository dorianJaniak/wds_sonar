#ifndef AUTHOR_H
#define AUTHOR_H

#include <QDialog>

namespace Ui {
class Author;
}

/*!
 * \brief Klasa Author
 *
 * Klasa odpowiada jedynie za wyświetlenie informacji o autorze.
 * W praktyce powstała nie tyle z potrzeby istnienia co z potrzeby
 * przetestowania otwierania nowego okna z poziomu MainWindow.
 */
class Author : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Konstruktor
     *
     * Ustawia jedyny widoczny tekst z opisem autorów.
     * \param[in] parent
     */
    explicit Author(QWidget *parent = 0);
    ~Author();


private:
    Ui::Author *ui;

protected:
    /*!
     * \brief Przeładowana funkcja showEvent
     *
     * Jedynie dostosowuje rozmiar okna do jego zawartości.
     */
    void showEvent(QShowEvent *);
};

#endif // AUTHOR_H

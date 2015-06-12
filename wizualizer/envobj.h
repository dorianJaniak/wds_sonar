#ifndef EnvObj_H
#define EnvObj_H

#include<QVector4D>
#include<QMatrix4x4>
#include<QVector>
#include <QOpenGLFunctions>


/**
 * @brief Klasa obiektu 3D związanego ze skanowaniem
 *
 * Klasa przechowuje komplet informacji potrzebnych do wyrysowania
 * jednego wyniku skanowania terenu.
 */
class EnvObj
{
    QVector<const GLfloat*> m_vertices;         ///<wektor tablic wierzchołków - jedna tablica odpowiada jednej części siatki
    QVector<unsigned int> m_verticesCount;      ///<wektor ilości wierzchołków w poszczególnych tablicach
    unsigned int m_allVertsCount;               ///<ilość wszystkich wierzchołków
    QVector4D m_centerPos;                      ///<punkt środkowy obiektu we współrzędnych jednorodnych
    QVector4D m_scale;                          ///<przeskalowanie poszczególnych osi
    QVector4D m_colorMaterial;                  ///<kolor materiału we współrzędnych RGBA
    float m_angleY;                             ///<obrót obiektu wokół własnej osi Y

public:
                            /**
     * @brief Konstruktor
     *
     * @param[in] verts wskaźnik na pełny zestaw wierzchołków potrzebnych do wyrysowania wyniku jednego skanowania
     * @param[in] color kolor materiału
     * @param[in] center punkt środkowy obiektu we współrzędnych jednorodnych (pozycja robota)
     */
    EnvObj(QVector<QVector<QVector4D>*>* verts, QVector4D color = QVector4D(), QVector4D center = QVector4D(0.0f,0.0f,0.0f,1.0f), float angleY=0.0f, QVector4D scale = QVector4D(1.0f,1.0f,1.0f,1.0f));
    ~EnvObj();
                            /**
     * @brief Zwraca ilość podsiatek, z których składa się obiekt
     * @return ilość podsiatek, z których składa się obiekt
     */
    unsigned int getMeshesCount();
                            /**
     * @brief weź tablicę wierzchołków jednej podsiatki
     * @param[in] meshIndex numer podsiatki
     * @return zwraca wskaźnik na tablicę gdy poprawny indeks, w przeciwnym wypadku nullptr
     */
    const float * getVerts(int meshIndex);
                            /**
     * @brief Pobranie ilości wierzchołków danej podsiatki
     * @param[in] meshIndex numer podsiatki
     * @return zwraca ilość wierzchołków lub 0 gdy niepoprawny numer indeksu
     */
    unsigned int getVertsCount(int meshIndex);
                            /**
     * @brief Pobierz ilość wszystkich wierzchołków
     * @return zwraca sumaryczną ilość wierzchołków wszystkich podsiatek
     */
    unsigned int getAllVertsCount();
                            /**
     * @brief Przygotowuje macierz translacji na podstawie m_centerPos
     * @return macierz 4x4 we współrzędnych jednorodnych
     */
    QMatrix4x4 getTranslationMatrix();
                            /**
     * @brief Pobiera kolor materiału
     * @return kolor materiału we współrzędnych RGBA
     */
    QVector4D getMaterialColor();

                            /**
     * @brief Ustawia środek obiektu
     * @param position środek we współrzędnych jednorodnych
     */
    void setCenter(QVector4D position);
                            /**
     * @brief Ustawia kąt obrotu wzdłuż osi Y
     * @param angleY kąt w stopniach
     */
    void setAngle(float angleY);
};

#endif // EnvObj_H

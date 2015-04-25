#ifndef ENVMAP_H
#define ENVMAP_H

#include<QVector4D>
#include<QMatrix4x4>
#include<QVector>
#include <QOpenGLFunctions>


/**
 * @brief Klasa obiektu 3D związanego ze skanowaniem
 * Klasa przechowuje komplet informacji potrzebnych do wyrysowania
 * jednego wyniku skanowania terenu.
 */
class EnvMap
{
    QVector<const GLfloat*> m_vertices;         ///@brief wektor tablic wierzchołków - jedna tablica odpowiada jednej części siatki
    QVector<unsigned int> m_verticesCount;      ///@brief wektor ilości wierzchołków w poszczególnych tablicach
    unsigned int m_allVertsCount;               ///@brief ilość wszystkich wierzchołków
    QVector4D m_centerPos;                      ///@brief punkt środkowy obiektu we współrzędnych jednorodnych
    QVector4D m_colorMaterial;                  ///@brief kolor materiału we współrzędnych RGBA

public:
                            /**
     * @brief Konstruktor
     * @param verts wskaźnik na pełny zestaw wierzchołków potrzebnych do wyrysowania wyniku jednego skanowania
     * @param color kolor materiału
     * @param center punkt środkowy obiektu we współrzędnych jednorodnych (pozycja robota)
     */
    EnvMap(QVector<QVector<QVector4D>*>* verts, QVector4D color = QVector4D(), QVector4D center = QVector4D(0.0f,0.0f,0.0f,1.0f));
    ~EnvMap();
                            /**
     * @brief Zwraca ilość podsiatek, z których składa się obiekt
     * @return ilość podsiatek, z których składa się obiekt
     */
    unsigned int getMeshesCount();
                            /**
     * @brief weź tablicę wierzchołków jednej podsiatki
     * @param meshIndex numer podsiatki
     * @return zwraca wskaźnik na tablicę gdy poprawny indeks, w przeciwnym wypadku nullptr
     */
    const float * getVerts(int meshIndex);
                            /**
     * @brief Pobranie ilości wierzchołków danej podsiatki
     * @param meshIndex numer podsiatki
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

};

#endif // ENVMAP_H

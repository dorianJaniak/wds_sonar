#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QObject>
#include <QWidget>
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "envobj.h"

class EnvObj;
typedef EnvObj EnvMap ;


/**
 * @brief Klasa zarządzająca widokiem 3D
 *
 * Klasa obsługuje renderowanie widoku 3D przy użyciu dziedziczonych klas
 * QOpenGLWidget oraz QOpenGLFunctions. Dzięki temu renderowany obraz może zostać
 * wyświetlony w kontrolce typu widżet.
 * Dodatkowo przechowuje komplet informacji o obiektach 3D, inicjalizuje OpenGL
 * oraz obsługuje zdarzenia myszy.
 */
class MapViewer : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
    QObject * m_parent;
public:
                        /**
     * @brief Konstruktor inicalizujący obiekty zagregowane w klasie oraz ustawiający parametry kamery
     * @param parent
     */
    MapViewer(QWidget *parent = 0);
    ~MapViewer();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

                        /**
     * @brief Dodaje obiekt przedstawiający wyniki skanowania otoczenia
     * @param verts kompletny zestaw wierzchołków
     * @param center punkt środkowy obiektu
     * @param allowToModifyY pozwolenie na "podnoszenie" kolejnych skanów względem osi Y
     */
    void addEnvMap(QVector<QVector<QVector4D> *> *verts, QVector4D center, bool allowToModifyY = true);
                        /**
     * @brief Dodaj robota, reprezentującego żądaną pozycję
     *
     * Przygotowuje OpenGL do wyrysowania obiektu 3D reprezentującego robota.
     * Powinna zostać wywołana tylko raz.
     */
    void addRequestRobot();
    /**
     * @brief Ustawia siatkę robota
     *
     * Robot jest reprezentowany przez obiekt 3D w oknie 3D.
     * Funkcja pozwala na załadowanie dowolnego obiektu.
     * @param verts wektor wierzchołków, będący siatką robota
     */
    void setRobotMesh(QVector<QVector4D>* verts);

    /**
     * @brief Zmień położenie robota, reprezentującego oczekiwaną pozycję
     * @param position oczekiwana pozycja we współrzędnych jednorodnych
     * @param angleY kąt obrotu wzdłuż osi Y
     */
    void setRequestRobotOrientation(QVector4D position, float angleY);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
                        /**
      * @brief Wylicza aspect ratio po zmianie rozmiaru okna
      */
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

                        /**
      * @brief Reaguje na ruch myszy
      * wylicza przesunięcie myszy względem ostatnio zapamiętanej pozycji
      */
    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
                        /**
      * @brief Zapamiętuje pozycję kursora
      */
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
                        /**
      * @brief Obsługuje zoom kamery
      */
    void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;

private:
                        /**
     * @brief Dodaje obiekt testowy
     * Tworzy VBO i VAO, rejestruje je i przypisuje im tablicę wierzchołków.
     */
    void addTestTriangle();
                        /**
     * @brief Dodaje siatkę
     * Generuje tablicę wierzchołków, które reprezentują siatkę. Tworzy dla nich VBO i VAO (gridVBO i gridVAO).
     * @param space bok pojedynczej kratki
     * @param rows ilość wierszy
     * @param cols ilość kolumn
     */
    void addGrid(float space = 1.0f, int rows = 10, int cols = 10);


                        /**
     * @brief Czyści środowisko
     */
    void clean();

                        /**
     * @brief Wylicza kolor materiału w zależnośći od numeru porządkowego obiektu.
     * Kolorowanie ma na celu odróżnienie poszczególnych obiektów na scenie.
     * @param objIndex numer porządkowy obiektu
     * @return kolor we współrzędnych RGBA.
     */
    QVector4D countColor(int objIndex);

    QOpenGLFunctions * f;

    //Shadery
    QOpenGLShaderProgram * _program;    ///<Wskaźnik na zarządcę shaderem ColorShader.
    GLuint _materialColorID;            ///<Uchwyt do zmiennej reprezentującej kolor materiału w shaderze ColorShader.


    //Ustawienie widoku
    QMatrix4x4 _projMat;                ///<Macierz projekcji
    QMatrix4x4 _centerMoveMat;          ///<Macierz przekształcenia całego widoku 3D (dla kamery).
    GLuint _projMatID;                  ///<Uchwyt zmiennej reprezentującej macierz projekcji w shaderze ColorShader.
    GLuint _centerMoveMatID;            ///<Uchwyt zmiennej reprezentującej macierz przekształcenia środka widoku 3D w shaderze ColorShader.
    GLfloat _cameraAngleX;              ///<Kąt wychylenia kamery względem osi X ( w stopniach ).
    GLfloat _cameraAngleY;              ///<Kąt wychylenia kamery względem osi Y ( w stopniach ).
    GLfloat _cameraFar;                 ///<Odległość kamery od środka sceny 3D.
    QPoint _mouseLastPos;               ///<Zapamiętana ostatnia pozycja myszy
    QVector4D m_backgroundColor;        ///<Kolor tła we współrzędnych jednorodnych

    //Przykladowy obiekt
    QOpenGLVertexArrayObject _vao;      ///<VAO testowego obiektu
    QOpenGLBuffer _vbo;                 ///<VBO testowego obiektu

    //Siatka - obiekt
    QOpenGLVertexArrayObject _gridVAO;  ///<VAO siatki
    QOpenGLBuffer _gridVBO;             ///<VBO siatki
    unsigned int _gridCountOfVerts;     ///<ilość wierzchołków siatki
    GLfloat * _gridVertices;            ///<tablica wierzchołków siatki
    QVector4D m_gridColor;              ///<kolor siatki w RGBA

    //Mapy - obiekty
    QVector<EnvMap*> m_maps;            ///<wektor obiektów 3D otoczenia
    QVector<QVector<QOpenGLVertexArrayObject*>> m_mapsVAOs; ///<zbiór VAO dla każdej podsiatki obiektów 3D otoczenia
    QVector<QVector<QOpenGLBuffer*>> m_mapsVBOs;            ///<zbiór VBO dla każdej podsiatki obiektów 3D otoczenia

    //Robot tymczasowy - gdy żądamy ruchu
    QVector<QVector4D>* m_robotMesh;     ///<Wskaźnik na wektor przechowujący siatkę obiektu 3D reprezentującego robota
    EnvObj* m_requestRobot;              ///<Obiekt robota tymczasowego, symbolizującego oczekiwaną pozycję robota
    QOpenGLVertexArrayObject* m_requestRobotVAO;            ///<VAO dla robota tymczasowego
    QOpenGLBuffer* m_requestRobotVBO;                       ///<VBO dla robota tymczasowego
    //New
    //QOpenGLShaderProgram * _mapProgram;
    //QOpenGLShaderProgram * _gridProgram;
    //QOpenGLShaderProgram * _robotProgram;
    //QVector<QOpenGLBuffer> _mapsVBO;      //Na jedno VAO przypadnie więcej niż jedno VBO, ponieważ siatka może być rozerwana (gdy duża odległość)

    //QOpenGLBuffer _robotVBO;
    //QVector<QOpenGLVertexArrayObject> _mapsVAO;

    //QOpenGLVertexArrayObject _robotVAO;

    //Do poprawy:


};

#endif // MAPVIEWER_H

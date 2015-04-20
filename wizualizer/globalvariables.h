#ifndef GLOBALVARIABLES
#define GLOBALVARIABLES

#include <QVector4D>
#include <QColor>

const float g_maxFar = 200.0f;  ///Pomiar ponad g_maxFar centymetrów będzie uznany za nieprawidłowy
const float g_yMapStep = 0.1f; ///Każda kolejna podmapa zostanie podniesiona o podaną wartość
const float g_colorStep = 0.1f; ///Zmiana koloru między kolejnymi mapami
const float g_gridLineWidth = 0.3f; ///Szerokość linii krawędzi
const float g_mapLineWidth = 1.0f; ///Szerokość linii mapy
const QVector4D g_startMapColor(0.3f,0.3f,1.0f,1.0f); ///Kolor względem, którego będzie wyliczany kolor kolejnej podmapy
const QVector4D g_backgroundColor(0.05f,0.05f,0.2f,1.0f); ///Domyślny kolor tła
const QVector4D g_gridColor(0.6f,0.6f,0.6f,1.0f); ///Domyślny kolor siatki






#endif // GLOBALVARIABLES

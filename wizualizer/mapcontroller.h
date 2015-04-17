#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include "globalvariables.h"

#include <QVector>


class MapController
{
    EnvMap _envMap;
    RawMap _rawMap;
public:
    MapController();
    ~MapController();

    void addData();
};

#endif // MAPCONTROLLER_H

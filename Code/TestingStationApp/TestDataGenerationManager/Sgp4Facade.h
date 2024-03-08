#ifndef SGP4FACADE_H
#define SGP4FACADE_H

#include "PointData.h"

class Sgp4Facade {
public:
    PointData sgp4(double t);
    PointData* sgp4Array(double t[]);
};

#endif
#ifndef PROG_POINT_H
#define PROG_POINT_H

#include "color.h"

class Point {
public:
    float x, y;
    color c;
    Point(float x_, float y_, const color& col)
        : x(x_), y(y_), c(col) {}
    Point(float x_, float y_)
        : x(x_), y(y_), c(255, 255, 255, 255) {}
};

#endif

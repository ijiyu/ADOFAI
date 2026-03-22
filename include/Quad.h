#ifndef PROG_QUAD_H
#define PROG_QUAD_H

#include "Vector2.h"
#include "color.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>

class Quad {
public:
    Vector2<float> p1, p2, p3, p4;
    color c;
    Vector2<float> min, max;
    Quad(Vector2<float> a, Vector2<float> b, Vector2<float> c_, Vector2<float> d, color col);

    void drawFilledTriangle(Vector2<float> a, Vector2<float> b, Vector2<float> c_) const;

    void display() const;
};

#endif
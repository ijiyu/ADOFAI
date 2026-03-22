#ifndef PROG_LINE_H
#define PROG_LINE_H

#include "color.h"
#include <SDL2/SDL.h>
#include "Vector2.h"
#include <algorithm>

class Line {
public:
    Vector2<float> start, end;
    color c;
    float length;
    Vector2<float> d, n;

    Line(Vector2<float> start, Vector2<float> end, color c);
    Line(Vector2<float> start, Vector2<float> end);

    void display() const;
};

#endif //PROG_LINE_H
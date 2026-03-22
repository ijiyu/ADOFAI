#ifndef PROG_RECT_H
#define PROG_RECT_H

#include <SDL2/SDL.h>
#include "color.h"
#include "Vector2.h"

class Rect {
public:
    float x, y, w, h;
    color c;

    Rect(float x, float y, float w, float h, color c);
    Rect(float x, float y, float w, float h);

    void display() const;
};

#endif //PROG_RECT_H
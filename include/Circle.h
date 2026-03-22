#ifndef PROG_CIRCLE_H
#define PROG_CIRCLE_H

#include <SDL2/SDL.h>
#include "color.h"
#include "Vector2.h"

extern SDL_Renderer *renderer;
extern Vector2<float> camera;

class Circle {
public:
    float x, y, r;
    color c;

    Circle(float x, float y, float r);

    Circle(float x, float y, float r, color c);

    void display() const;
};



#endif //PROG_CIRCLE_H
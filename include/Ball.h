#ifndef PROG_BALL_H
#define PROG_BALL_H

extern float deltaTime;

#include "color.h"
#include "Circle.h"

class Ball {
public:
    Vector2<float> pos;
    float r;
    bool rotating;
    color c{};

    Ball *other = nullptr;

    float w = 2;
    float theta = 0;

    Ball(Vector2<float> pos, float r, bool rotating, color c);

    void display() const;
    void update();
};

#endif //PROG_BALL_H
#include "Circle.h"

Circle::Circle(float x, const float y, const float r)
    : x(x), y(y), r(r) {
    c = color(255, 255, 255, 255);
}

Circle::Circle(float x, const float y, const float r, const color c)
    : x(x), y(y), r(r), c(c) {
}

void Circle::display() const {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    const int radius = static_cast<int>(r);
    for (int dy = -radius; dy <= radius; dy++)
        for (int dx = -radius; dx <= radius; dx++)
            if (dx * dx + dy * dy <= radius * radius)
                SDL_RenderDrawPoint(renderer, static_cast<int>(x - camera.x) + dx, static_cast<int>(y - camera.y) + dy);
}
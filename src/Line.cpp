#include "Line.h"

extern SDL_Renderer* renderer;
extern Vector2<float> camera;

Line::Line(const Vector2<float> start, const Vector2<float> end, const color c)
    : start(start), end(end), c(c) {
    length = std::sqrt((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y));
    d = end - start;
    n = Vector2<float>(-d.y / length, d.x / length);
}

Line::Line(const Vector2<float> start, const Vector2<float> end)
    : start(start), end(end) {
    c = color(255, 255, 255, 255);
    length = std::sqrt((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y));
    d = end - start;
    n = Vector2<float>(-d.y / length, d.x / length);
}

void Line::display() const {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    int thickness = 4;

    for (int i = -thickness/2; i <= thickness/2; i++) {
        Vector2<float> o = n*i;

        SDL_RenderDrawLine(
            renderer,
            start.x - camera.x + o.x,
            start.y - camera.y + o.y,
            end.x - camera.x + o.x,
            end.y - camera.y + o.y
        );
    }
}
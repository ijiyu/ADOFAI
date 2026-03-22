#include "Tile.h"

Tile::Tile(const Vector2<float> start1, const Vector2<float> start2, const Vector2<float> mid1, const Vector2<float> mid2,
           const Vector2<float> end1, const Vector2<float> end2, const std::string& type, const color c)
    : start1(start1), start2(start2), mid1(mid1), mid2(mid2), end1(end1), end2(end2), type(type), c(c) {
    if (type == "flip")
        this->c = {255, 0, 255, 255};
    if (type == "fast")
        this->c = {255, 0, 0, 255};
    if (type == "slow")
        this->c = {0, 0, 255, 255};
}

void Tile::display() const {
    const Quad q1(start1, start2, mid2, mid1, c);
    const Quad q2(mid1, mid2, end2, end1, c);
    const color outline(0, 0, 0, 255);
    const Line l1(start1, start2, outline);
    const Line l2(start2, mid2, outline);
    const Line l3(mid2, end2, outline);
    const Line l4(end2, end1, outline);
    const Line l5(end1, mid1, outline);
    const Line l6(mid1, start1, outline);
    q1.display();
    q2.display();
    l1.display();
    l2.display();
    l3.display();
    l4.display();
    l5.display();
    l6.display();
};
bool Tile::isVisible(const Vector2<float>& camera, int screenW, int screenH) const {
    float minX = std::min({start1.x, start2.x, mid1.x, mid2.x, end1.x, end2.x});
    float maxX = std::max({start1.x, start2.x, mid1.x, mid2.x, end1.x, end2.x});
    float minY = std::min({start1.y, start2.y, mid1.y, mid2.y, end1.y, end2.y});
    float maxY = std::max({start1.y, start2.y, mid1.y, mid2.y, end1.y, end2.y});

    float screenLeft   = camera.x;
    float screenRight  = camera.x + screenW;
    float screenTop    = camera.y;
    float screenBottom = camera.y + screenH;

    return !(maxX < screenLeft ||
             minX > screenRight ||
             maxY < screenTop ||
             minY > screenBottom);
}
void Tile::displayNoCamera() const {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    SDL_RenderDrawLine(renderer, start1.x, start1.y, start2.x, start2.y);
    SDL_RenderDrawLine(renderer, start2.x, start2.y, mid2.x, mid2.y);
    SDL_RenderDrawLine(renderer, mid2.x, mid2.y, end2.x, end2.y);
    SDL_RenderDrawLine(renderer, end2.x, end2.y, end1.x, end1.y);
    SDL_RenderDrawLine(renderer, end1.x, end1.y, mid1.x, mid1.y);
    SDL_RenderDrawLine(renderer, mid1.x, mid1.y, start1.x, start1.y);
}
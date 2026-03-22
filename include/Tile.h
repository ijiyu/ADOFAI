#ifndef PROG_TILE_H
#define PROG_TILE_H

#include <string>

#include "Vector2.h"
#include "color.h"
#include "Quad.h"
#include "Line.h"

extern SDL_Renderer *renderer;

class Tile {
public:
    Vector2<float> start1, start2, mid1, mid2, end1, end2, connection;
    std::string type;
    color c{};
    Tile(Vector2<float> start1, Vector2<float> start2, Vector2<float> mid1, Vector2<float> mid2,
         Vector2<float> end1, Vector2<float> end2, const std::string& type, color c);

    void display() const;
    bool isVisible(const Vector2<float>& camera, int screenW, int screenH) const;
    void displayNoCamera() const;
};


#endif //PROG_TILE_H
#include "Quad.h"

extern SDL_Renderer* renderer;
extern Vector2<float> camera;

Quad::Quad(Vector2<float> a, Vector2<float> b, Vector2<float> c_, Vector2<float> d, color col)
    : p1(a), p2(b), p3(c_), p4(d), c(col) {
}

void Quad::display() const {
    SDL_Vertex verts[6];

    Vector2<float> camOffset = camera;

    verts[0].position = {p1.x - camera.x, p1.y - camera.y};
    verts[1].position = {p2.x - camera.x, p2.y - camera.y};
    verts[2].position = {p3.x - camera.x, p3.y - camera.y};
    verts[3].position = {p1.x - camera.x, p1.y - camera.y};
    verts[4].position = {p3.x - camera.x, p3.y - camera.y};
    verts[5].position = {p4.x - camera.x, p4.y - camera.y};

    for(auto & vert : verts) {
        vert.color = {static_cast<Uint8>(c.r), static_cast<Uint8>(c.g), static_cast<Uint8>(c.b), static_cast<Uint8>(c.a)};
        vert.tex_coord = {0,0};
    }

    SDL_RenderGeometry(renderer, nullptr, verts, 6, nullptr, 0);
}
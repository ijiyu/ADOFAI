#ifndef PROG_TEXT_H
#define PROG_TEXT_H

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "color.h"
#include "Vector2.h"
#include <string>

extern SDL_Renderer *renderer;
extern Vector2<float> camera;

class Text {
public:
    std::string text;
    float x, y;
    SDL_Color c;
    TTF_Font *font;
    bool absolutePosition;

    Text(std::string text, float x, float y, SDL_Color c, TTF_Font *font, bool absolutePosition);

    void display();
};

#endif //PROG_TEXT_H
#include "../include/Text.h"

#include <utility>

Text::Text(std::string text, const float x, const float y, const SDL_Color c, TTF_Font *font, const bool absolutePosition)
    : text(std::move(text)), x(x), y(y), c(c), font(font), absolutePosition(absolutePosition) {}

void Text::display() {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), c);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {
        static_cast<int>(x) - surface->w / 2 - static_cast<int>(camera.x) * absolutePosition, static_cast<int>(y) - surface->h / 2 - static_cast<int>(camera.y) * absolutePosition, surface->w, surface->h
    };

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
}

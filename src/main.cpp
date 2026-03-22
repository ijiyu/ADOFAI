#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "color.h"
#include "Vector2.h"
#include "Circle.h"
#include "Ball.h"
#include "Tile.h"
#include "Text.h"

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 1000;
const Vector2<float> SCREEN_MIDPOINT = Vector2<float>(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0f;
constexpr int FPS = 120;
constexpr int FRAME_DELAY = 1000 / FPS;

constexpr float BPM = 210.0f;
constexpr float PERIOD = 60.0f / BPM;
constexpr float SNAP_DISTANCE = 120.0f;

const auto BACKGROUND_COLOR = color(177, 230, 224, 255);
const auto TILE_COLOR = color(154, 219, 200, 255);

constexpr float CAMERA_SPEED = 3.0f;
constexpr float PI = 3.14159265358979323846f;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
Mix_Music *music = nullptr;
Vector2<float> camera(-100.f, 0.f);
bool running = true;
SDL_Event event;
float deltaTime = 0.0f;

constexpr float circleRadius = 30;
constexpr float orbitRadius = 2.25f * 2 * circleRadius;
constexpr float tileWidth = orbitRadius;
constexpr float tileHeight = circleRadius * 2.2f;

Vector2<float> startPoint(200, 800);
Ball red({startPoint.x, startPoint.y}, circleRadius, false, {255, 0, 0, 255});
Ball blue({startPoint.x - orbitRadius, startPoint.y}, circleRadius, true, {0, 0, 255, 255});

std::vector<Vector2<float> > points;
std::vector<Tile> tiles;
int currentIndex;
int startPosOffset = 0;

float countdownTimer = 0.0f;
int lastCountdownValue = 3;
bool countingDown = true;

float fpsTimer = 0.0f;
int frameCount = 0;

bool inputQueued = false;
Uint32 inputTime = 0;
constexpr int INPUT_DELAY_MS = 10;

TTF_Font *font200;
TTF_Font *font50;

std::vector<Text> texts;
int erasedTexts = 0;

void drawGrid() {
    constexpr int spacing = 100;
    for (int x = SCREEN_WIDTH * -10; x < SCREEN_WIDTH * 10; x += spacing) {
        const int screenX = x - static_cast<int>(camera.x);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, screenX, 0, screenX, SCREEN_HEIGHT);
    }
    for (int y = SCREEN_HEIGHT * -10; y < SCREEN_HEIGHT * 10; y += spacing) {
        const int screenY = y - static_cast<int>(camera.y);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawLine(renderer, 0, screenY, SCREEN_WIDTH, screenY);
    }
}

std::vector<std::pair<float, std::string> > readLevel(const std::string &filename) {
    std::vector<std::pair<float, std::string> > data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float angle;
        std::string command;
        iss >> angle;
        if (!(iss >> command))
            command = "";
        data.emplace_back(angle, command);
    }
    return data;
}

Vector2<float> rotatePoint(const Vector2<float> &p, const Vector2<float> &origin, const float angle) {
    const float x = p.x - origin.x;
    const float y = p.y - origin.y;
    const float xr = x * std::cos(angle) - y * std::sin(angle);
    const float yr = x * std::sin(angle) + y * std::cos(angle);
    return {xr + origin.x, yr + origin.y};
}

Tile buildTile(const Vector2<float> initialPos, float innerAngle, float initialAngle, const std::string &type) {
    innerAngle = innerAngle * PI / 180.0f;
    initialAngle = initialAngle * PI / 180.0f;
    const float innerSupplement = 2 * PI - innerAngle;
    const Vector2 botCenter = initialPos;
    const Vector2 midCenter(botCenter.x, botCenter.y - tileWidth / 2);
    const Vector2 topCenter(tileWidth / 2 * std::sin(innerSupplement) + midCenter.x,
                            tileWidth / 2 * std::cos(innerSupplement) + midCenter.y);
    Vector2 bot1(botCenter.x - tileHeight / 2, botCenter.y);
    Vector2 bot2(botCenter.x + tileHeight / 2, botCenter.y);
    Vector2 top1(topCenter.x + tileHeight / 2 * std::cos(innerSupplement),
                 topCenter.y - tileHeight / 2 * std::sin(innerSupplement));
    Vector2 top2(topCenter.x - tileHeight / 2 * std::cos(innerSupplement),
                 topCenter.y + tileHeight / 2 * std::sin(innerSupplement));
    Vector2 mid1(midCenter.x - tileHeight / 2, midCenter.y + tileHeight / 2 / (std::tan(innerAngle / 2)));
    Vector2 mid2(midCenter.x + tileHeight / 2, midCenter.y - tileHeight / 2 / (std::tan(innerAngle / 2)));
    // midCenter and topCenter aren't rotated
    bot1 = rotatePoint(bot1, botCenter, initialAngle);
    bot2 = rotatePoint(bot2, botCenter, initialAngle);
    top1 = rotatePoint(top1, botCenter, initialAngle);
    top2 = rotatePoint(top2, botCenter, initialAngle);
    mid1 = rotatePoint(mid1, botCenter, initialAngle);
    mid2 = rotatePoint(mid2, botCenter, initialAngle);
    const Tile tile(bot1, bot2, mid1, mid2, top1, top2, type, TILE_COLOR);
    return tile;
}

void start() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    SDL_CreateWindowAndRenderer(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_RENDERER_PRESENTVSYNC,
        &window,
        &renderer
    );
    SDL_StartTextInput();
    TTF_Init();

    font200 = TTF_OpenFont("godoMaum.ttf", 200);
    font50 = TTF_OpenFont("godoMaum.ttf", 50);

    red.other = &blue;
    blue.other = &red;
    currentIndex = startPosOffset + 1;

    Vector2<float> currPos = startPoint;
    float currAngle = 90;
    for (auto level = readLevel("level10.txt"); const auto &[innerAngle, type]: level) {
        Tile tile = buildTile(currPos, innerAngle, currAngle, type);
        tiles.push_back(tile);
        currPos = (tile.end1 + tile.end2) * 0.5f;
        currAngle = currAngle - 180 + innerAngle;
        points.push_back((tile.mid1 + tile.mid2) * 0.5f);
    }

    red.pos = points[startPosOffset];
    blue.pos = points[startPosOffset] + Vector2<float>(orbitRadius, 0);
    red.w = BPM / 60.0f * PI;
    blue.w = BPM / 60.0f * PI;

    camera = points[startPosOffset] - SCREEN_MIDPOINT;
}

void countdown() {
    countdownTimer += deltaTime;
    if (countdownTimer >= PERIOD) {
        countdownTimer -= PERIOD;

        lastCountdownValue--;

        if (lastCountdownValue == 0) {
            music = Mix_LoadMUS("ButterflyPlanet.mp3");
            if (!music) std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
            if (Mix_PlayMusic(music, -1) == -1) std::cout << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
        }
        if (lastCountdownValue < 0) {
            countingDown = false;
        }
    }
}

void handleInput() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            if (event.key.repeat == 0) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                    case SDLK_LCTRL:
                    case SDLK_RCTRL:
                    case SDLK_LALT:
                    case SDLK_RALT:
                        break;
                    default: ;
                }
                inputQueued = true;
                inputTime = SDL_GetTicks();
            }
        }
    }
}

void move() {
    inputQueued = false;
    Ball *pivot;
    Ball *orbit;
    if (red.rotating) {
        pivot = &blue;
        orbit = &red;
    } else {
        pivot = &red;
        orbit = &blue;
    }
    const Vector2<float> target = points[currentIndex];
    const Tile &tile = tiles[currentIndex];
    float dist = (orbit->pos - target).magnitude();

    if (dist <= SNAP_DISTANCE) {
        std::string timingType = "";
        SDL_Color color;
        if (dist <= 40) {
            timingType = "Perfect";
            color = {0, 255, 0, 255};
        } else if (dist <= 80) {
            timingType = "Almost";
            color = {200, 255, 0, 255};
        } else {
            timingType = "Off";
            color = {255, 0, 0, 255};
        }

        Text text(timingType, pivot->pos.x, pivot->pos.y - 50, color, font50, true);
        texts.push_back(text);

        const Vector2<float> d = orbit->pos - target;
        const Vector2<float> tangent = Vector2(-(orbit->pos.y - pivot->pos.y), (orbit->pos.x - pivot->pos.x)).
                normalized();
        const float arcError = d.dot(tangent);
        const float angleError = arcError / orbitRadius;
        orbit->pos = target;
        if (tile.type == "flip") {
            orbit->w *= -1;
            pivot->w *= -1;
        } else if (tile.type == "fast") {
            const float sign = std::abs(orbit->w) / orbit->w;
            orbit->w = sign * BPM / 60.0f * PI;
            pivot->w = sign * BPM / 60.0f * PI;
        } else if (tile.type == "slow") {
            const float sign = std::abs(orbit->w) / orbit->w;
            orbit->w = sign * 5.3f;
            pivot->w = sign * 5.3f;
        }
        pivot->rotating = true;
        orbit->rotating = false;
        pivot->theta = std::atan2((pivot->pos - orbit->pos).y, (pivot->pos - orbit->pos).x) + angleError;
        currentIndex++;
    } else {
        std::cout << "Bad" << std::endl;
    }
}

void updateCamera() {
    Vector2<float> cameraTarget;
    if (red.rotating) cameraTarget = blue.pos - SCREEN_MIDPOINT;
    else cameraTarget = red.pos - SCREEN_MIDPOINT;
    camera += (cameraTarget - camera) * CAMERA_SPEED * deltaTime;
}

void draw() {
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);

    int visibleRange = 20;
    for (int i = currentIndex - visibleRange; i <= currentIndex + visibleRange; ++i) {
        if (i >= 0 && i < tiles.size() && tiles[i].isVisible(camera, SCREEN_WIDTH, SCREEN_HEIGHT))
            tiles[i].display();
    }

    red.display();
    blue.display();

    if (countingDown) {
        std::string displayedString = (lastCountdownValue >= 1) ? std::to_string(lastCountdownValue) : "Go!";
        Text text(displayedString, SCREEN_MIDPOINT.x, SCREEN_MIDPOINT.y, {255, 255, 255, 255}, font200, false);
        text.display();
    }

    for (int i = erasedTexts; i < texts.size(); i++) {
        Text &text = texts[i];
        text.display();

        if (text.c.a > 2)
            text.c.a -= 2;
        else
            text.c.a = 0;

        if (text.c.a == 0) {
            erasedTexts++;
        }
    }

    SDL_RenderPresent(renderer);
}

void update() {
    if (countingDown)
        countdown();
    handleInput();
    if (inputQueued && SDL_GetTicks() - inputTime >= INPUT_DELAY_MS)
        move();
    updateCamera();
    red.update();
    blue.update();
    draw();
}

void end() {
    Mix_FreeMusic(music);
    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    start();
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        const Uint32 frameStart = SDL_GetTicks();
        deltaTime = static_cast<float>(frameStart - lastTime) / 1000.0f;
        lastTime = frameStart;
        fpsTimer += deltaTime;
        frameCount++;

        if (fpsTimer >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        update();

        if (const Uint32 frameTime = SDL_GetTicks() - frameStart; frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    end();
    return 0;
}
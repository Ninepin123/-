#include "Coin.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Coin::Coin(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer)
    : GameObject(x, y, w, h, texturePath, renderer), originalX(x), collected(false) {
    // Initialize random number generator
    std::srand(static_cast<unsigned>(std::time(0)));
}

Coin::~Coin() {
    // Resource cleanup is done in the base class destructor
}

void Coin::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

bool Coin::isCollected() const {
    return collected;
}

void Coin::collect() {
    collected = true;
}

void Coin::setX(int x) {
    rect.x = x;
}

void Coin::setY(int y) {
    rect.y = y;
}

int Coin::getOriginalX() const {
    return originalX;
}

void Coin::randomizePosition(int screenWidth, int screenHeight) {
    int x = std::rand() % (screenWidth - rect.w);
    int y = std::rand() % (screenHeight - rect.h);
    rect.x = x;
    rect.y = y;
}

void Coin::reset(int screenWidth, int screenHeight) {
    collected = false;
    randomizePosition(screenWidth, screenHeight);
}

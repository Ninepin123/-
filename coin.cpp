#include "Coin.h"
#include <iostream>

Coin::Coin(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer)
    : rect{ x, y, w, h }, collected(false) {
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        std::cout << "Failed to load coin image: " << IMG_GetError() << std::endl;
        texture = nullptr;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cout << "Failed to create coin texture: " << SDL_GetError() << std::endl;
    }
}

Coin::~Coin() {
    SDL_DestroyTexture(texture);
}

void Coin::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

SDL_Rect Coin::getRect() const {
    return rect;
}

bool Coin::isCollected() const {
    return collected;
}

void Coin::collect() {
    collected = true;
}

#include "GameObject.h"
#include <iostream>

GameObject::GameObject(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer) {
    rect = { x, y, w, h };
    texture = IMG_LoadTexture(renderer, texturePath.c_str());
    if (!texture) {
        std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
    }
}

GameObject::~GameObject() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

SDL_Rect GameObject::getRect() const {
    return rect;
}

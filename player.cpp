#include "Player.h"
#include <iostream>
#include <SDL2/SDL_image.h>

Player::Player(int startX, int startY, int w, int h, int moveSpeed, SDL_Color c)
    : GameObject(startX, startY, w, h, "", nullptr), speed(moveSpeed), color(c) {
    texture = nullptr;
}

void Player::move(const Uint8* keyState, int screenWidth, int screenHeight) {
    if (keyState[SDL_SCANCODE_UP]) rect.y -= speed;
    if (keyState[SDL_SCANCODE_DOWN]) rect.y += speed;
    if (keyState[SDL_SCANCODE_LEFT]) rect.x -= speed;
    if (keyState[SDL_SCANCODE_RIGHT]) rect.x += speed;

    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.w > screenWidth) rect.x = screenWidth - rect.w;
    if (rect.y < 0) rect.y = 0;
    if (rect.y + rect.h > screenHeight) rect.y = screenHeight - rect.h;
}

// Ensure this matches the pure virtual function in GameObject exactly
void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        // If texture is available, render it
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    } else {
        // If no texture is available, use the color for rendering a filled rectangle
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Player::loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cout << "Failed to load texture: " << IMG_GetError() << std::endl;
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Player::checkCollision(const SDL_Rect& other) const {
    return SDL_HasIntersection(&rect, &other);
}

#include "Player.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Constructor
Player::Player(int startX, int startY, int w, int h, int moveSpeed, SDL_Color c)
    : x(startX), y(startY), width(w), height(h), speed(moveSpeed), color(c), texture(nullptr) {
}

// Move the player
void Player::move(const Uint8* keyState, int screenWidth, int screenHeight) {
    if (keyState[SDL_SCANCODE_UP]) {
        y -= speed;  // Move up by one step
        if (y < 0) {
            y = 0;  // Prevent going above the screen
        }
        // After moving, reset to the starting position
        x = 100;  // Reset to initial x position
        y = 250;  // Reset to initial y position
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        y += speed;  // Move down by one step
        if (y + height > screenHeight) {
            y = screenHeight - height;  // Prevent going below the screen
        }
        // After moving, reset to the starting position
        x = 100;  // Reset to initial x position
        y = 250;  // Reset to initial y position
    }
    if (keyState[SDL_SCANCODE_LEFT]) x -= speed;
    if (keyState[SDL_SCANCODE_RIGHT]) x += speed;

    // Prevent going outside the screen bounds
    if (x < 0) x = 0;
    if (x + width > screenWidth) x = screenWidth - width;
    if (y < 0) y = 0;
    if (y + height > screenHeight) y = screenHeight - height;
}

// Render the player
void Player::render(SDL_Renderer* renderer) const {
    if (texture) {
        SDL_Rect destRect = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_Rect rect = { x, y, width, height };
        SDL_RenderFillRect(renderer, &rect);
    }
}

// Load texture for the player
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

// Check for collision with another rectangle
bool Player::checkCollision(const SDL_Rect& other) const {
    SDL_Rect playerRect = { x, y, width, height };
    return SDL_HasIntersection(&playerRect, &other);
}

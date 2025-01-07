#include "coin.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor to initialize the coin's position and load the texture
Coin::Coin(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer)
    : originalX(x), collected(false) {
    rect = { x, y, w, h };  // Initialize the SDL_Rect for the coin
    texture = IMG_LoadTexture(renderer, texturePath.c_str());
    if (!texture) {
        std::cout << "Failed to load coin texture: " << SDL_GetError() << std::endl;
    }

    // Initialize random number generator
    std::srand(static_cast<unsigned>(std::time(0)));
}

// Destructor to clean up the texture
Coin::~Coin() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

// Render the coin if it is not collected
void Coin::render(SDL_Renderer* renderer) {
    if (!collected) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect); // Draw the coin at its current position
    }
}

// Get the current position of the coin
SDL_Rect Coin::getRect() const {
    return rect;
}

// Check if the coin has been collected
bool Coin::isCollected() const {
    return collected;
}

// Mark the coin as collected
void Coin::collect() {
    collected = true;
}

// Set the current x position of the coin (for scrolling effect)
void Coin::setX(int x) {
    rect.x = x; // Update the coin's x position based on the scroll offset
}

// Set the current y position of the coin
void Coin::setY(int y) {
    rect.y = y;
}

// Get the original x position of the coin (before scrolling)
int Coin::getOriginalX() const {
    return originalX; // Return the coin's original x position
}

// Randomize the coin's position within the screen bounds
void Coin::randomizePosition(int screenWidth, int screenHeight) {
    int x = std::rand() % (screenWidth - rect.w);  // Ensure coin is within screen bounds
    int y = std::rand() % (screenHeight - rect.h); // Ensure coin is within screen bounds
    rect.x = x;
    rect.y = y;
}

// Reset the coin's state and position
void Coin::reset(int screenWidth, int screenHeight) {
    collected = false;  // Reset collection flag
    randomizePosition(screenWidth, screenHeight);  // Randomize position
}

// Get the width of the coin
int Coin::getWidth() const {
    return rect.w;
}

// Get the height of the coin
int Coin::getHeight() const {
    return rect.h;
}

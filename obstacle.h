#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>  // For error logging

using namespace std;

class Obstacle {
public:
    Obstacle(int x, int y, int w, int h, const char* texturePath, SDL_Renderer* renderer)
        : originalX(x), x(x), y(y), width(w), height(h), texture(nullptr) {
        texture = loadTexture(renderer, texturePath);
    }

    ~Obstacle() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    void setX(int newX) {
        x = newX;
    }

    int getOriginalX() const {
        return originalX;
    }

    SDL_Rect getRect() {
        return { x, y, width, height };
    }

    void render(SDL_Renderer* renderer) {
        if (texture) {
            SDL_Rect rect = getRect();
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        } else {
            cerr << "Error: Failed to load texture for obstacle." << endl;
        }
    }

private:
    int originalX, x, y, width, height;
    SDL_Texture* texture;

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
        SDL_Surface* surface = IMG_Load(filePath);
        if (!surface) {
            cerr << "Error: Could not load image " << filePath << " - " << IMG_GetError() << endl;
            return nullptr;  // Return nullptr if the image fails to load
        }

        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!newTexture) {
            cerr << "Error: Could not create texture from surface - " << SDL_GetError() << endl;
        }

        SDL_FreeSurface(surface);  // Free the surface after creating the texture
        return newTexture;  // Return the texture (may be nullptr if creation failed)
    }
};

#endif

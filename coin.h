#ifndef COIN_H
#define COIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Coin {
private:
    SDL_Rect rect;        // Current position and size of the coin
    SDL_Texture* texture; // Texture of the coin
    bool collected;       // Flag indicating if the coin has been collected
    int originalX;        // The original x position of the coin (before scrolling)

public:
    Coin(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    ~Coin();

    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    bool isCollected() const;
    void collect();
    void setX(int x);         // Set the current x position of the coin
    void setY(int y);         // Set the current y position of the coin
    int getOriginalX() const; // Get the original x position of the coin

    // New function for randomizing coin position
    void randomizePosition(int screenWidth, int screenHeight);

    // Reset the coin's state and position
    void reset(int screenWidth, int screenHeight);

    // New methods to get the width and height
    int getWidth() const;
    int getHeight() const;
};

#endif

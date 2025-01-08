#ifndef COIN_H
#define COIN_H

#include "GameObject.h"
#include <SDL2/SDL.h>
#include <string>
#include <cstdlib>
#include <ctime>

class Coin : public GameObject {
private:
    bool collected;       // Flag indicating if the coin has been collected
    int originalX;        // The original x position of the coin (before scrolling)

public:
    Coin(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    ~Coin();

    void render(SDL_Renderer* renderer) override;
    bool isCollected() const;
    void collect();
    void setX(int x);
    void setY(int y);
    int getOriginalX() const;
    
    // New function for randomizing coin position
    void randomizePosition(int screenWidth, int screenHeight);

    // Reset the coin's state and position
    void reset(int screenWidth, int screenHeight) override;
};

#endif

#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <SDL2/SDL.h>
#include <string>

class Player : public GameObject {
private:
    int speed;          // Player's movement speed
    SDL_Color color;    // Player's color (used for non-textured rendering)

public:
    Player(int startX, int startY, int w, int h, int moveSpeed, SDL_Color c);
    void move(const Uint8* keyState, int screenWidth, int screenHeight);
    void render(SDL_Renderer* renderer) override;

    bool loadTexture(SDL_Renderer* renderer, const char* filePath);
    bool checkCollision(const SDL_Rect& other) const;

    void reset(int screenWidth, int screenHeight) override {
        // For simplicity, Player does not need reset functionality (or it could be extended as needed)
    }
};

#endif

#ifndef COIN_H
#define COIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Coin {
private:
    SDL_Rect rect;
    SDL_Texture* texture;
    bool collected;

public:
    Coin(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    ~Coin();

    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    bool isCollected() const;
    void collect();
};

#endif

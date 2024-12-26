
#include <SDL2/SDL.h>

#ifndef PLAYER_H
#define PLAYER_H


class Player {
private:
    int x, y;           
    int width, height;  
    int speed;          
    SDL_Color color;
public:
    Player(int startX, int startY, int w, int h, int moveSpeed, SDL_Color c);

    void move(const Uint8* keyState, int screenWidth, int screenHeight);


    void render(SDL_Renderer* renderer) const;
};

#endif // PLAYER_H

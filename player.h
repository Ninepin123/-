#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player {
private:
    int x, y;           
    int width, height;  
    int speed;          
    SDL_Color color;    
    SDL_Texture* texture; // 小恐龍圖片

public:
    Player(int startX, int startY, int w, int h, int moveSpeed, SDL_Color c);
    ~Player();

    bool loadTexture(SDL_Renderer* renderer, const char* filePath); // 加載圖片
    void move(const Uint8* keyState, int screenWidth, int screenHeight);
    void render(SDL_Renderer* renderer) const;
};

#endif // PLAYER_H

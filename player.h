#include <SDL2/SDL.h>
#include<string.h>
#ifndef PLAYER_H
#define PLAYER_H

// Player ���O�w�q
class Player {
private:
    int x, y;           // �D������m
    int width, height;  // �D�����ؤo
    int speed;          // �D�������ʳt��
    SDL_Texture* texture;
    SDL_Color color; 

public:
    // �غc�禡
    Player(int startX, int startY, int w, int h, int moveSpeed, SDL_Color c);
    // �B�z�����޿�
    void move(const Uint8* keyState, int screenWidth, int screenHeight);

    void render(SDL_Renderer* renderer) const;

    bool checkCollision(const SDL_Rect& other) const;

    bool loadTexture(SDL_Renderer* renderer, const char* filePath);
};

#endif 
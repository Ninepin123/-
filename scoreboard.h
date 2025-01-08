#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

class Scoreboard {
public:
    Scoreboard(int startX, int startY, SDL_Color color, TTF_Font* font);
    void addScore(int points);
    void resetScore();
    void render(SDL_Renderer* renderer) const;
    void renderGameOver(SDL_Renderer* renderer, TTF_Font* font) const;
    int getScore() const { return score; }

private:
    int score;
    int x, y;
    SDL_Color color;
    TTF_Font* font;
};

#endif

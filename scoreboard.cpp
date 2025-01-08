#include "Scoreboard.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

Scoreboard::Scoreboard(int startX, int startY, SDL_Color c, TTF_Font* font)
    : x(startX), y(startY), color(c), font(font), score(0) {
}

void Scoreboard::addScore(int points) {
    score += points;
}

void Scoreboard::resetScore() {
    score = 0;
}

void Scoreboard::render(SDL_Renderer* renderer) const {
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(texture);
}

void Scoreboard::renderGameOver(SDL_Renderer* renderer, TTF_Font* font) const {
    // Render the Game Over text
    SDL_Color red = { 255, 0, 0 };
    std::string gameOverText = "Game Over! Final Score: " + std::to_string(score);
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), red);
    if (gameOverSurface == nullptr) {
        std::cout << "Unable to render 'Game Over' text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect = { 400 - gameOverSurface->w / 2, 300 - gameOverSurface->h / 2, gameOverSurface->w, gameOverSurface->h };
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);

    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);
}

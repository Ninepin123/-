#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream> 
#include "player.h"
#include "scoreboard.h"
#include "coin.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Texture* loadBackground(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("mario run", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void close(SDL_Window*& window, SDL_Renderer*& renderer,TTF_Font*& font) {
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y) {
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text, color);
    if (!surface) {
        std::cout << "Failed to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cout << "Failed to create text texture! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Color red = { 255, 0, 0 };
    SDL_Color white = { 255, 255, 255 };
    SDL_Color black = { 0, 0, 0 };
    if (!init(window, renderer)) {
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return -1;
    }
    TTF_Font* font = TTF_OpenFont("font/Arial.ttf", 24); // Make sure to use the correct path to your font
    if (!font) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        close(window, renderer, font);
        return -1;
    }

    Player player(100, 500, 50, 50, 1, red);
    Scoreboard scoreboard(10, 10, black, font);
    Coin coin(200, 400, 40, 40, "asset/coin.jpg", renderer);
    Coin coin1(400, 300, 40, 40, "asset/coin1.jpg", renderer);

    SDL_Texture* backgroundTexture = loadBackground(renderer, "asset/background.png");
    if (!backgroundTexture) {
        close(window, renderer, font);
        return -1;
    }
    if (!player.loadTexture(renderer, "asset/dino.png")) {
        std::cout << "Failed to load player texture!" << std::endl;
        close(window, renderer, font);
        return -1;
    }
    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* keyState = SDL_GetKeyboardState(nullptr);
        player.move(keyState, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (!coin.isCollected() && player.checkCollision(coin.getRect())) {
            coin.collect();
            scoreboard.addScore(5);
        }
        if (!coin1.isCollected() && player.checkCollision(coin1.getRect())) {
            coin1.collect();
            scoreboard.addScore(10);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        scoreboard.render(renderer);
        player.render(renderer);
        coin.render(renderer);
        coin1.render(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    close(window, renderer,font);
    return 0;
}
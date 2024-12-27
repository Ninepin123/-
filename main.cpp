#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "player.h"

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
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("dinosour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

void close(SDL_Window*& window, SDL_Renderer*& renderer, TTF_Font*& font) {
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
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

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        return -1;
    }

    // 加載字體
    TTF_Font* font = TTF_OpenFont("asset/msjh.ttc", 24);
    if (!font) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        close(window, renderer, font);
        return -1;
    }

    SDL_Texture* backgroundTexture = loadBackground(renderer, "asset/background.png");
    if (!backgroundTexture) {
        close(window, renderer, font);
        return -1;
    }

    SDL_Color white = {255, 255, 255};
    Player player(100, 500, 50, 50, 1, white);

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

        // 清空畫面
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 背景黑色
        SDL_RenderClear(renderer);

        // 渲染背景
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        // 渲染 "分數" 文字
        renderText(renderer, font, "分數", white, 10, 10);

        // 渲染主角
        player.render(renderer);

        // 更新畫面
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    close(window, renderer, font);
    return 0;
}

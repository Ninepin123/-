#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "player.h"
#include "scoreboard.h"
#include "coin.h"
#include "obstacle.h"
#include "timer.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
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
    window = SDL_CreateWindow("Mario Run with Scroll", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    SDL_Quit();
}

bool checkOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    return SDL_HasIntersection(&rect1, &rect2);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color textColor = {0, 0, 0};  // Black color.
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_DestroyTexture(textTexture);
}

void renderStartScreen(SDL_Renderer* renderer, SDL_Texture* startTexture) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White background
    SDL_RenderFillRect(renderer, nullptr);

    SDL_Rect startRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};  // Full-screen image
    SDL_RenderCopy(renderer, startTexture, nullptr, &startRect);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) {
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("font/Arial.ttf", 24);
    if (!font) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        close(window, renderer, font);
        return -1;
    }

    // Initialize player, scoreboard, and coins
    Player player(100, 3500, 50, 50, 1, {255, 0, 0});
    Scoreboard scoreboard(10, 10, {0, 0, 0}, font);
    Coin coin(800, 400, 50, 50, "asset/coin.jpg", renderer);
    Coin coin1(750, 300, 50, 50, "asset/coin1.jpg", renderer);

    // Initialize obstacles
    Obstacle obstacle1(500, 100, 80, 60, "asset/obstacle.png", renderer);
    Obstacle obstacle2(600, 400, 80, 60, "asset/obstacle.png", renderer);
    Obstacle obstacle3(700, 250, 80, 60, "asset/obstacle.png", renderer);

    // Load background and game state textures
    SDL_Texture* backgroundTexture = loadTexture(renderer, "asset/background.png");
    SDL_Texture* successTexture = loadTexture(renderer, "asset/success.png");
    SDL_Texture* gameOverTexture = loadTexture(renderer, "asset/gameover.png");
    SDL_Texture* startTexture = loadTexture(renderer, "asset/start.png");  // Load the start screen image

    if (!backgroundTexture || !successTexture || !gameOverTexture || !startTexture) {
        close(window, renderer, font);
        return -1;
    }

    if (!player.loadTexture(renderer, "asset/dino.png")) {
        std::cout << "Failed to load player texture!" << std::endl;
        close(window, renderer, font);
        return -1;
    }

    // Timer initialization
    Timer gameTimer;
    gameTimer.start();

    bool running = true;
    bool gameOver = false;
    bool gameStarted = false;  // Track if the game has started
    SDL_Event e;

    // Scrolling background variables
    float scrollOffset = 0.0f;
    const float SCROLL_SPEED = 0.6f;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            if (!gameStarted && e.type == SDL_KEYDOWN) {
                gameStarted = true;  // Start the game when any key is pressed
                gameTimer.start();  // Restart the timer when game starts
            }
        }

        if (!gameStarted) {
            // Show start screen
            renderStartScreen(renderer, startTexture);
            continue;
        }

        if (!gameOver) {
            const Uint8* keyState = SDL_GetKeyboardState(nullptr);
            player.move(keyState, SCREEN_WIDTH, SCREEN_HEIGHT);

            // Update coin positions
            coin.setX(coin.getOriginalX() + scrollOffset);
            coin1.setX(coin1.getOriginalX() + scrollOffset);

            // Check for overlap and reposition coins if needed
            SDL_Rect coinRect = coin.getRect();
            SDL_Rect coin1Rect = coin1.getRect();
            while (checkOverlap(coinRect, obstacle1.getRect()) ||
                   checkOverlap(coinRect, obstacle2.getRect()) ||
                   checkOverlap(coinRect, obstacle3.getRect())) {
                coin.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                coinRect = coin.getRect(); // Update coinRect after reset
            }

            while (checkOverlap(coin1Rect, obstacle1.getRect()) ||
                   checkOverlap(coin1Rect, obstacle2.getRect()) ||
                   checkOverlap(coin1Rect, obstacle3.getRect())) {
                coin1.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                coin1Rect = coin1.getRect(); // Update coin1Rect after reset
            }

            // Update obstacle positions
            obstacle1.setX(obstacle1.getOriginalX() + scrollOffset);
            obstacle2.setX(obstacle2.getOriginalX() + scrollOffset);
            obstacle3.setX(obstacle3.getOriginalX() + scrollOffset);

            // Check for coin collisions and update score
            if (!coin.isCollected() && player.checkCollision(coin.getRect())) {
                coin.collect();
                scoreboard.addScore(5);
            }
            if (!coin1.isCollected() && player.checkCollision(coin1.getRect())) {
                coin1.collect();
                scoreboard.addScore(10);
            }

            // Check for collisions with obstacles
            if (player.checkCollision(obstacle1.getRect()) ||
                player.checkCollision(obstacle2.getRect()) ||
                player.checkCollision(obstacle3.getRect())) {
                gameOver = true;
                gameTimer.stop(); // Stop the timer if player hits an obstacle
            }

            // Clear screen with white background
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color
            SDL_RenderFillRect(renderer, nullptr);  // Fill the entire screen with white

            // Scrolling background logic
            scrollOffset -= SCROLL_SPEED;
            if (scrollOffset <= -SCREEN_WIDTH) {
                scrollOffset += SCREEN_WIDTH;

                // Reset coins if collected
                if (coin.isCollected()) {
                    coin.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                }
                if (coin1.isCollected()) {
                    coin1.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                }

                // Reset obstacles
                obstacle1.setX(SCREEN_WIDTH + rand() % 200);
                obstacle2.setX(SCREEN_WIDTH + 200 + rand() % 200);
                obstacle3.setX(SCREEN_WIDTH + 400 + rand() % 200);
            }

            // Render scrolling background
            SDL_Rect srcRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Rect destRect1 = {static_cast<int>(scrollOffset), 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Rect destRect2 = {static_cast<int>(scrollOffset) + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect1);
            SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect2);

            // Render game objects
            scoreboard.render(renderer);
            player.render(renderer);
            coin.render(renderer);
            coin1.render(renderer);
            obstacle1.render(renderer);
            obstacle2.render(renderer);
            obstacle3.render(renderer);

            // Render elapsed time
            Uint32 elapsedTime = gameTimer.getTicks();
            renderText(renderer, font, "Time: " + std::to_string(elapsedTime / 1000) + "s", SCREEN_WIDTH - 150, 10);
        } else {
            // Display end game screen
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, nullptr);

            // Render end screen text
            Uint32 elapsedTime = gameTimer.getTicks();
            renderText(renderer, font, "Score: " + std::to_string(scoreboard.getScore()), 10, 10);
            renderText(renderer, font, "Time: " + std::to_string(elapsedTime / 1000) + "s", SCREEN_WIDTH - 150, 10);

            // Wait for quit event or key press
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
                    running = false;
                }
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(successTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(startTexture);  // Clean up start screen texture
    close(window, renderer, font);
    return 0;
}
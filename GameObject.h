#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class GameObject {
protected:
    SDL_Rect rect;        // Position and size of the object
    SDL_Texture* texture; // Texture for the object

public:
    GameObject(int x, int y, int w, int h, const std::string& texturePath, SDL_Renderer* renderer);
    virtual ~GameObject();

    virtual void render(SDL_Renderer* renderer) = 0; // Pure virtual method for rendering
    SDL_Rect getRect() const;

    virtual void reset(int screenWidth, int screenHeight) = 0; // Pure virtual method to reset the object
};

#endif

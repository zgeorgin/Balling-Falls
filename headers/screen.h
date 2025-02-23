#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <vector>

class Screen
{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Screen(int width, int height, const char windowName[] = "Untitled");
    Screen(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer) {}
    ~Screen();
    void Draw();
    SDL_Event CheckInput();
};

#endif
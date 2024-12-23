#ifndef SCREEN_H
#define SCREEN_H

#include <drawing-features.h>
#include <iostream>
#include <memory>
#include <vector>

using PrimitivePtr = std::shared_ptr<Primitive>; 
class Screen
{
public:
    std::vector<PrimitivePtr> objects;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Screen(int width, int height, const char windowName[] = "Untitled");
    Screen(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer) {}
    ~Screen();
    void AddObject(PrimitivePtr object) { objects.push_back(object);} 
    void Draw();
    SDL_Event CheckInput();
};

#endif
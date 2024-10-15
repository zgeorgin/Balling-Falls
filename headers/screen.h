#ifndef SCREEN_H
#define SCREEN_H

#include <primitives.h>
#include <memory>
#include <vector>

using PrimitivePtr = std::shared_ptr<Primitive>; 
class Screen
{
public:
    std::vector<PrimitivePtr> objects;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Screen(int width, int height, char windowName[] = "Untitled");
    Screen(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer) {}
    ~Screen();
    void AddObject(PrimitivePtr object) { objects.push_back(object);} 
    void Draw();
    SDL_Event CheckInput();
};

#endif
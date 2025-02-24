#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
struct RGB
{
    uint8_t r, g, b;
};

struct Position
{
    int x, y;
};

class Circle
{
private:
    size_t radius;
    Position center;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int originalWidth, originalHeight;
    RGB color;

public:
    const char* filepath = "../img/circle.png";
    Circle() {}
    Circle(size_t radius, Position center, RGB color, SDL_Renderer* renderer, SDL_Texture* texture = nullptr);
    void draw (SDL_Renderer* new_renderer, SDL_Texture* texture);
    void changePosition(Position new_center) {center = new_center;}
    Position getPosition() {return center;}
    size_t getRadius() {return radius;}
    RGB getColor() {return color;}
    void removeTexture() {SDL_DestroyTexture(texture);}
    //~Circle() {SDL_DestroyTexture(texture);}
};
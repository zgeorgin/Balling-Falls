#include <circle.h>

Circle::Circle(size_t radius, Position center, RGB color, SDL_Renderer* renderer, SDL_Surface* surface) : radius(radius), center(center), color(color), renderer(renderer)
{
    if (!surface)
        SDL_Surface* surface = IMG_Load(filepath);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_FreeSurface(surface);

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_QueryTexture(texture, nullptr, nullptr, &originalWidth, &originalHeight);
}

void Circle::draw(SDL_Renderer* new_renderer)
{   
    SDL_Rect srcRect = {0, 0, originalWidth, originalHeight}; 
    SDL_Rect destRect = {center.x - radius, center.y - radius, radius * 2, radius * 2}; 

    SDL_RenderCopy(new_renderer, texture, &srcRect, &destRect);
}

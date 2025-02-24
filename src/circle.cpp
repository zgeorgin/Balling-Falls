#include <circle.h>

Circle::Circle(size_t radius, Position center, RGB color, SDL_Renderer* renderer, SDL_Texture* texture) : radius(radius), center(center), color(color), renderer(renderer)
{
    /*f (!surface)
        SDL_Surface* surface = IMG_Load(filepath);*/
    SDL_QueryTexture(texture, nullptr, nullptr, &originalWidth, &originalHeight);
    //texture = texture;
}

void Circle::draw(SDL_Renderer* new_renderer, SDL_Texture* srcTexture)
{   
    SDL_SetTextureColorMod(srcTexture, color.r, color.g, color.b);

    SDL_Rect srcRect = {0, 0, originalWidth, originalHeight}; 
    SDL_Rect destRect = {center.x - radius, center.y - radius, radius * 2, radius * 2}; 

    SDL_RenderCopy(new_renderer, srcTexture, &srcRect, &destRect);
}

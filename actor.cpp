#include "actor.h"

void BallActor::UpdateLink()
{
    int32_t x = int32_t(body->position.x * scale);
    int32_t y = int32_t(body->position.y * scale);
    image->centreX = x;
    image->centreY = y;
    std::cout << x << ' ' << image->centreX << std::endl;
}

void BallActor::Draw(SDL_Renderer* renderer)
{
    image->Draw(renderer);
}
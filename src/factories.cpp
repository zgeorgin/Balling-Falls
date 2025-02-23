#include <factories.h>

std::shared_ptr<BallActor> BallActorFactory::BuildRandom(int maxRadius, int minRadius, bool isColored, int scale, size_t id, Position velocity, SDL_Renderer* renderer)
{
    float radius = rand()%(maxRadius - minRadius) + minRadius;
    uint8_t r = rand()%255, g = rand()%255, b = rand()%255;
    float x = radius;
    float y = radius;
    Ball* ball = new Ball(radius, radius, {x, y}, {x + velocity.x, y + velocity.y}, id);
    std::shared_ptr<Circle> image = std::make_shared<Circle>(Circle(radius, {ball->position.x * scale, ball->position.y * scale}, {r, g, b}, renderer));
    return std::make_shared<BallActor>(BallActor(ball, image, scale));
}
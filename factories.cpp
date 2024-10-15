#include "factories.h"

std::shared_ptr<BallActor> BallActorFactory::BuildRandom(int maxRadius, int minRadius, bool isColored, int scale)
{
    double radius = rand()%(maxRadius - minRadius) + minRadius;
    uint8_t r = rand()%255, g = rand()%255, b = rand()%255;
    double x = radius;
    double y = radius;
    std::shared_ptr<Ball> ball = std::make_shared<Ball>(Ball(radius, radius, {x, y}, {x - 5, y}));
    int32_t circle_x = int32_t(ball->position.x * scale);
    int32_t circle_y = int32_t(ball->position.y * scale);
    std::shared_ptr<Circle> image = std::make_shared<Circle>(circle_x, circle_y, ball->radius, false, r, g, b,255);
    return std::make_shared<BallActor>(BallActor(ball, image, scale));
}
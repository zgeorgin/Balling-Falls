#ifndef FACTORIES_H
#define FACTORIES_H
#include <actor.h>
#include <random>

class BodyFactory
{
public:
    std::shared_ptr<Body> Build (long double mass, Vector2 position, Vector2 prev_position) {return std::make_shared<Body>(Body(mass, position, prev_position));}
};

class BallActorFactory
{
public:
    std::shared_ptr<BallActor> BuildRandom(int maxRadius, int minRadius, bool isColored = true, int scale = 1, size_t id = 0, Position velocity = {0, 0},SDL_Renderer* renderer = nullptr);
};

#endif
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <memory>
#include <cmath>
#include <omp.h>
const long double G = 6.67e-11;
const long double g = 9.8;

struct Vector2
{
    double x, y;
    Vector2(double x, double y) : x(x), y(y) {}
    Vector2() : x(0), y(0) {}
};

class Body
{
public:
    long double mass;
    Vector2 prev_position;
    Vector2 position;
    Body(long double mass, Vector2 position, Vector2 prev_position) : position(position), prev_position(prev_position), mass(mass) {}
    void UpdatePosition(Vector2 acceleration, double delta);
};

class Ball : public Body
{
public:
    int radius;
    double elasticity = 0.9;
    Ball(long double mass, int radius, Vector2 position, Vector2 prev_position) : radius(radius), Body(mass, position, prev_position) {}
};

class Arena
{
public:
    double delta = 1;
    std::vector<std::shared_ptr<Body>> objects;
    int animationTicks = 0;
    virtual void ApplyForces() = 0;
    Arena(std::vector<std::shared_ptr<Body>> vec) : objects(vec) {}
    Arena() {}
};

class BallCollideArena : public Arena
{
public:
    uint32_t uborder, dborder, rborder, lborder;
    std::vector<std::shared_ptr<Ball>> objects;
    virtual void ApplyForces() override;
    void ApplyGravity();
    void HandleCollisions();
    void UpdatePositions(double dt);
    BallCollideArena(std::vector<std::shared_ptr<Ball>> vec, uint32_t u, uint32_t d, uint32_t l, uint32_t r) : objects(vec), uborder(u), dborder(d), lborder(l), rborder(r) {}
};

#endif
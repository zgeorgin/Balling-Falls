#ifndef ARENA_H
#define ARENA_H
#include <physics.h>

class Arena
{
public:
    double delta = 1;
    std::vector<std::shared_ptr<Body>> objects;
    int animationTicks = 0;
    virtual void ApplyForces() = 0;
    void AddObject(std::shared_ptr<Body> object) {objects.push_back(object);};
    //Arena(std::vector<std::shared_ptr<Body>> vec) : objects(vec) {}
    //Arena() {}
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
    void AddObject(std::shared_ptr<Ball> object) {objects.push_back(object);}
    BallCollideArena(std::vector<std::shared_ptr<Ball>> vec, uint32_t u, uint32_t d, uint32_t l, uint32_t r) : objects(vec), uborder(u), dborder(d), lborder(l), rborder(r) {}
};

#endif
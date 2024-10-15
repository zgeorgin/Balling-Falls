#ifndef ACTOR_H
#define ACTOR_H

#include "primitives.h"
#include "physics.h"

// Class, that links image with physical body
class Actor
{
public:
    int scale;
    std::shared_ptr<Body> body;
    std::shared_ptr<Primitive> image;
    //Actor(std::shared_ptr<Body> body, std::shared_ptr<Primitive> image, int scale = 1) : body(body), image(image), scale(scale) {}
    //Actor() {}
    virtual void UpdateLink() = 0;
    virtual void Draw(SDL_Renderer* renderer) = 0;
};

class BallActor : public Actor
{
public:
    int scale;
    std::shared_ptr<Ball> body;
    std::shared_ptr<Circle> image;
    BallActor(std::shared_ptr<Ball> body, std::shared_ptr<Circle> image, int scale = 1) : body(body), image(image), scale(scale){}
    ~BallActor() {std::cout << "BALLACTOR DELETED!!!!";}
    void UpdateLink() override;
    void Draw(SDL_Renderer* renderer) override;
};

#endif
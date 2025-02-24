#ifndef ACTOR_H
#define ACTOR_H

#include <circle.h>
#include <physics.h>
#include <string.h>
#include <sstream>

// Class, that links image with physical body
/*class Actor
{
public:
    int scale;
    std::shared_ptr<Body> body;
    std::shared_ptr<Primitive> image;
    //Actor(std::shared_ptr<Body> body, std::shared_ptr<Primitive> image, int scale = 1) : body(body), image(image), scale(scale) {}
    //Actor() {}
    virtual void UpdateLink() = 0;
    virtual void Draw(SDL_Renderer* renderer) = 0;
};/**/

class BallActor
{
public:
    int scale;
    Ball* body;
    std::shared_ptr<Circle> image;
    BallActor(Ball* body, std::shared_ptr<Circle> image, int scale = 1) : body(body), image(image), scale(scale){}
    BallActor(std::string saveString, int scale = 1);
    ~BallActor() {}
    void UpdateLink();
    void Draw(SDL_Renderer* renderer, SDL_Texture* texture);
    std::string getSaveString();
};

#endif
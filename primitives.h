#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>
#include <iostream>
class Primitive
{
public:
    Uint8 r, g, b, a;
    Primitive() : r(255), g(255), b(255), a(255) {}
    Primitive(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) {}
    ~Primitive() {};
    virtual void Draw(SDL_Renderer* renderer) = 0;
    void ChangeColor(Uint8 r, Uint8 g, Uint8 b) {this->r = r; this->g = g; this->b = b;};
};

class Circle : public Primitive
{
public:
    int32_t centreX, centreY, radius;
    bool painted;
    Circle() {}
    Circle(int32_t x, int32_t y, int32_t radius, bool painted = false, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255):
     centreX(x), centreY(y), radius(radius), painted(painted), Primitive(r, g, b, a) {};
    ~Circle() {}
    void Draw(SDL_Renderer* renderer) override;
private:
    void DrawCircle(SDL_Renderer* renderer);
    void FillCircle(SDL_Renderer* renderer);
};

class Rect : public Primitive
{
public:
    int32_t x1, x2, y1, y2;
    bool painted;
    Rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, bool painted = false, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255):
     x1(x1), x2(x2), y1(y1), y2(y2),  painted(painted), Primitive(r, g, b, a) {};
    ~Rect() {};
    void Draw(SDL_Renderer* renderer) override;
private:
    void DrawRect(SDL_Renderer* renderer);
    void FillRect(SDL_Renderer* renderer);
};

class Triangle : public Primitive
{
public:
    int32_t x1, x2, x3, y1, y2, y3;
    bool painted;
    Triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, bool painted = false, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255):
     x1(x1), x2(x2), y1(y1), y2(y2),  x3(x3), y3(y3), painted(painted), Primitive(r, g, b, a) {};
    ~Triangle() {};
    void Draw(SDL_Renderer* renderer)  override;
private:
    void DrawTriangle(SDL_Renderer* renderer);
};

class Grid : public Primitive
{
public:
    int32_t offset;
    Grid(int32_t offset, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255) : offset(offset), Primitive(r, g, b, a) {};
    ~Grid() {};
    void Draw(SDL_Renderer* renderer, int height, int width);
};

#endif
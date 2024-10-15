#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <memory>
#include <cmath>
#include <omp.h>
const float G = 6.67e-11;
const float g = 9.8;

struct Vector2
{
    float x, y;
    Vector2(float x, float y) : x(x), y(y) {}
    Vector2() : x(0), y(0) {}
};

struct Vector3
{
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3() : x(0), y(0), z(0) {}
};

float norm(Vector2 a);

Vector2 operator+ (const Vector2& a, const Vector2& b) ;
Vector2 operator* (const Vector2& a, float b);
Vector2 operator* (float b, const Vector2& a);
Vector2 operator/ (const Vector2& a, float b);
Vector2 operator- (const Vector2& a, const Vector2& b) ;
Vector2 operator+ (float a, const Vector2& b) ;
Vector2 operator+ (const Vector2& b, float a) ;
Vector2 operator- (float a, const Vector2& b) ;
Vector2 operator- (const Vector2& b, float a) ;

class Body
{
public:
    float mass;
    Vector2 prev_position;
    Vector2 position;
    Body(float mass, Vector2 position, Vector2 prev_position) : position(position), prev_position(prev_position), mass(mass) {}
    void UpdatePosition(Vector2 acceleration, float delta);
};

class Ball : public Body
{
public:
    int radius;
    float elasticity = 0.9;
    Ball(float mass = 1, int radius = 1, Vector2 position = {0, 0}, Vector2 prev_position = {0, 0}) : radius(radius), Body(mass, position, prev_position) {}
};

#endif
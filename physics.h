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

struct Vector3
{
    double x, y, z;
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3() : x(0), y(0), z(0) {}
};

double norm(Vector2 a);

Vector2 operator+ (const Vector2& a, const Vector2& b) ;
Vector2 operator* (const Vector2& a, double b);
Vector2 operator* (double b, const Vector2& a);
Vector2 operator/ (const Vector2& a, double b);
Vector2 operator- (const Vector2& a, const Vector2& b) ;
Vector2 operator+ (double a, const Vector2& b) ;
Vector2 operator+ (const Vector2& b, double a) ;
Vector2 operator- (double a, const Vector2& b) ;
Vector2 operator- (const Vector2& b, double a) ;

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
    Ball(long double mass = 1, int radius = 1, Vector2 position = {0, 0}, Vector2 prev_position = {0, 0}) : radius(radius), Body(mass, position, prev_position) {}
};

#endif
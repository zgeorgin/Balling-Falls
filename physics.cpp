#include "physics.h"
#include <iostream>

double norm(Vector2 a) { return sqrt(a.x * a.x + a.y * a.y); }

Vector2 operator+ (const Vector2& a, const Vector2& b) { return Vector2(a.x + b.x, a.y + b.y); }
Vector2 operator* (const Vector2& a, double b) { return Vector2(a.x * b, a.y * b); }
Vector2 operator* (double b, const Vector2& a) { return Vector2(a.x * b, a.y * b); }
Vector2 operator/ (const Vector2& a, double b) { return Vector2(a.x / b, a.y / b); }
Vector2 operator- (const Vector2& a, const Vector2& b) { return Vector2(a.x - b.x, a.y - b.y); }
Vector2 operator+ (double a, const Vector2& b) { return Vector2(a + b.x, a + b.y); }
Vector2 operator+ (const Vector2& b, double a) { return Vector2(a + b.x, a + b.y); }
Vector2 operator- (double a, const Vector2& b) { return Vector2(a - b.x, a - b.y); }
Vector2 operator- (const Vector2& b, double a) { return Vector2(b.x - a, b.y - a); }

std::ostream& operator<<(std::ostream& out, Vector2 a)
{
    out << a.x << ' ' << a.y;
    return out;
}

void Body::UpdatePosition(Vector2 acceleration, double delta)
{
    Vector2 oldPos = position;
    position = 2*position - prev_position + acceleration * delta * delta;
    prev_position = oldPos;
} 



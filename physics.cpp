#include "physics.h"
#include <iostream>

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

double norm(Vector2 a) { return sqrt(a.x * a.x + a.y * a.y); }

void Body::UpdatePosition(Vector2 acceleration, double delta)
{
    Vector2 oldPos = position;
    position = 2*position - prev_position + acceleration * delta * delta;
    prev_position = oldPos;
} 

void BallCollideArena :: ApplyGravity()
{
    #pragma omp parallel for
        for (int i = 0; i < objects.size(); i++)
        {
            double dx = 0, dy = 0;
            if (objects[i]->position.y + objects[i]->radius > dborder)
                objects[i]->position.y = dborder - objects[i]->radius;

            if (objects[i]->position.y - objects[i]->radius < uborder)
                objects[i]->position.y = uborder + objects[i]->radius;

            if (objects[i]->position.x - objects[i]->radius < lborder)
                objects[i]->position.x = lborder + objects[i]->radius;

            if (objects[i]->position.x + objects[i]->radius > rborder)
                objects[i]->position.x = rborder - objects[i]->radius;
        }
}

void BallCollideArena :: HandleCollisions()
{
    #pragma omp parallel for collapse(2)
        for (int i = 0; i < objects.size(); i++)
        {
            for (int j = 0; j < objects.size(); j++)
            {
                if (i == j) continue;
                Vector2 direction = (objects[i]->position - objects[j]->position);
                double dist = norm(direction);
                double min_dist =  objects[i]->radius + objects[j]->radius;
                if (dist < min_dist)
                {
                    Vector2 direction_norm = direction / dist;
                    double interval = min_dist - dist;
                    Vector2 diff = direction_norm * interval / 2.0;
                    objects[i]->position = objects[i]->position + diff; // * objects[i]->mass / (objects[i]->mass + objects[j]->mass);    
                    objects[j]->position = objects[j]->position - diff; //* objects[j]->mass / (objects[i]->mass + objects[j]->mass);       
                }
            }
        }
}

void BallCollideArena :: UpdatePositions(double dt)
{
    #pragma omp parallel for
    for (int i = 0; i < objects.size(); i++)
    {
       objects[i]->UpdatePosition({0, 10000}, dt);
    }
}

void BallCollideArena :: ApplyForces()
{
    for (int k = 0; k < animationTicks; k++)
    { 
        ApplyGravity();
        HandleCollisions();
        UpdatePositions(delta/animationTicks);
    }
}

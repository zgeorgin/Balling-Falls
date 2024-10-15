#include "arena.h"
#include <iostream>
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
    std::cout << 'sdgdsfgd\n';
    for (int k = 0; k < animationTicks; k++)
    { 
        ApplyGravity();
        HandleCollisions();
        UpdatePositions(delta/animationTicks);
    }
}
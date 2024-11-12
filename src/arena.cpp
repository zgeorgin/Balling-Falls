#include <arena.h>
#include <iostream>

bool Cell::inBorders(BallPtr ball)
{
    return (ball->position.x <= rb) && (ball->position.x >= lb) && (ball->position.y <= db) && (ball->position.y >= ub);
}

void Cell::fillCell(std::vector<BallPtr> allBalls)
{
    balls = {};
    for (BallPtr b : allBalls)
    {
        if (inBorders(b))
            balls.push_back(b);
    }
}

bool solveCollision(BallPtr ball1, BallPtr ball2)
{
    if (ball1->id == ball2->id) return false;
    Vector2 direction = (ball1->position - ball2->position);
    double dist = norm(direction);
    double min_dist =  ball1->radius + ball2->radius;
    if (dist < min_dist)
    {
        Vector2 direction_norm = direction / dist;
        double interval = min_dist - dist;
        Vector2 diff = direction_norm * interval / 2.0;
        ball1->position = ball1->position + diff; // * objects[i]->mass / (objects[i]->mass + ball2->mass);    
        ball2->position = ball2->position - diff; //* objects[j]->mass / (objects[i]->mass + objects[j]->mass);     
        return true;  
    }
    return false;
}
void Cell::collideWithCell(std::shared_ptr<Cell> another)
{
    bool is_collide = true;
    while (is_collide)
    {
        is_collide = false;
        for (int i = 0; i < balls.size(); i++)
        {
            for (int j = 0; j < another->balls.size(); j++)
            {
                BallPtr ball1 = balls[i];
                BallPtr ball2 = another->balls[j];
                solveCollision(ball1, ball2);
            }
        }
    }

}

BallCollideArena::BallCollideArena(std::vector<BallPtr> vec, uint32_t u, uint32_t d, uint32_t l, uint32_t r, size_t cellCountX, size_t cellCountY): objects(vec), uborder(u), dborder(d), lborder(l), rborder(r) 
{
    cells = std::vector<std::vector<CellPtr>>(cellCountY + 2, std::vector<CellPtr>(cellCountX + 2, std::make_shared<Cell>(Cell(0, 0, 0, 0))));
    for (int i = 1; i < cells.size() - 1; i++)
    {
        for (int j = 1; j < cells[i].size() - 1; j++)
        {
            cells[i][j]->ub = (i == 1) ? u : (d - u) / cellCountY * (i - 1);
            cells[i][j]->db = (i == cells.size() - 2) ? d : (d - u) / cellCountY * i;
            cells[i][j]->lb = (j == 1) ? l : (r - l) / cellCountX * (j - 1);
            cells[i][j]->rb = (j == cells[i].size() - 2) ? r : (r - l) / cellCountX * j; 
            std::cout << cells[i][j]->ub << ' ' << cells[i][j]->db << ' ' << cells[i][j]->lb << ' ' << cells[i][j]->rb << '\n';
        }
    }
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
    for (int i = 1; i < cells.size() - 1; i++)
    {
        for (int j = 1; j < cells[i].size() - 1; j++)
            cells[i][j]->fillCell(objects);
    }

    //#pragma omp parallel for collapse(2)
    for (int i = 1; i < cells.size() - 1; i++)
    {
        for (int j = 1; j < cells[i].size() - 1; j++)
        {
            cells[i][j]->collideWithCell(cells[i][j]);
            for (int k = -1; k <= 1; k++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    cells[i][j]->collideWithCell(cells[i + k][j + m]);
                }
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
        //std::cout << "GRAVITY APPLIED!\n";
        UpdatePositions(delta/animationTicks);
    }
}
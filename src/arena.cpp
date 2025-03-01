#include <arena.h>
#include <iostream>
#include <thread>
inline bool Cell::inBorders(Ball* ball)
{
    const float x = ball->position.x;
    const float y = ball->position.y;

    if ((y > db) || (y < ub))
        return false;

    if ((x > rb) || (x < lb))
        return false;
    
    return true; 
    //return (x <= rb) && (x >= lb) && (y <= db) && (y >= ub);
}

void Cell::fillCell(std::vector<Ball*> allBalls)
{
    balls = {};
    for (Ball* b : allBalls)
    {
        if (inBorders(b))
            balls.push_back(b);
    }
}

bool solveCollision(Ball* ball1, Ball* ball2)
{
    if (ball1->id == ball2->id) return false;
    Vector2 direction = (ball1->position - ball2->position);
    double dist = norm(direction);
    double min_dist = ball1->radius + ball2->radius;
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
void collideCells(const Cell& cell1, const Cell& cell2)
{
    bool is_collide = true;
    //#pragma omp parallel for collapse(2)
    for (int i = 0; i < cell1.balls.size(); i++)
    {
        for (int j = 0; j < cell2.balls.size(); j++)
        {
            Ball* ball1 = cell1.balls[i];
            Ball* ball2 = cell2.balls[j];
            solveCollision(ball1, ball2);
        }
    }
}

BallCollideArena::BallCollideArena(std::vector<Ball*> vec, uint32_t u, uint32_t d, uint32_t l, uint32_t r, size_t cellCountX, size_t cellCountY): objects(vec), uborder(u), dborder(d), lborder(l), rborder(r) 
{
    cellCols = cellCountX + 2;
    cellRows = cellCountY + 2;
    cellHight = (d - u) / cellCountY;
    cellWidth = (r - l) / cellCountX;
    //cells = std::vecot
    cells = std::vector<std::vector<Cell>>(cellCountY + 2, std::vector<Cell>(cellCountX + 2));
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[0].size(); j++)
        {
            cells[i][j] = Cell(0, 0, 0, 0);
            if ((i > 0) && (i < cells.size() - 1) && (j > 0) && (j < cells[i].size() - 1))
            {
                cells[i][j].ub = (i == 1) ? u : (d - u) / cellCountY * (i - 1);
                cells[i][j].db = (i == cells.size() - 2) ? d : (d - u) / cellCountY * i;
                cells[i][j].lb = (j == 1) ? l : (r - l) / cellCountX * (j - 1);
                cells[i][j].rb = (j == cells[i].size() - 2) ? r : (r - l) / cellCountX * j; 
            }
        }
    }
}

void BallCollideArena :: ApplyGravity()
{
    //#pragma omp parallel for
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
void processColsStatic(const std::vector<std::vector<Cell>>& cells, size_t startCol, size_t endCol)
{
    for (int i = 1; i <= cells.size() - 1; i++)
    {
        for (int j = startCol; j <= endCol; j++)
        {
            if (j >= cells[i].size())
                break;
            if (cells[i][j].balls.empty())
                continue;
            for (int k = -1; k <= 1; k++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    if (cells[i + k][j + m].balls.empty())
                        continue;
                    collideCells(cells[i][j], cells[i + k][j + m]);
                }
            }
        }
    }
}
void processRowsStatic(const std::vector<std::vector<Cell>>& cells, size_t startRow, size_t endRow)
{
    for (int i = startRow; i <= endRow; i++)
    {
        if (i >= cells.size())
            break;
        for (int j = 1; j < cells[0].size() - 1; j++)
        {
            if (cells[i][j].balls.empty())
                continue;
            for (int k = -1; k <= 1; k++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    if (cells[i + k][j + m].balls.empty())
                        continue;
                    collideCells(cells[i][j], cells[i + k][j + m]);
                }
            }
        }
    }
}

void BallCollideArena :: proccessRows(size_t startRow, size_t endRow)
{
    for (int i = startRow; i < endRow; i++)
    {
        if (i >= cellRows)
            break;
        for (int j = 1; j < cellCols - 1; j++)
        {
            if (cells[i][j].balls.empty())
                continue;
            for (int k = -1; k <= 1; k++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    if (cells[i + k][j + m].balls.empty())
                        continue;
                    collideCells(cells[i][j], cells[i + k][j + m]);
                }
            }
        }
    }
}

void BallCollideArena :: HandleCollisions()
{
    /*for (int i = 1; i < cells.size() - 1; i++) ТАК БЫЛО РАНЬШЕ - ТАК БОЛЬШЕ НЕ НАДО!!!
    {
        for (int j = 1; j < cells[i].size() - 1; j++)
            cells[i][j]->fillCell(objects);
    }*/

    for (int i = 1; i < cellRows - 1; i++)
    {
        for (int j = 1; j < cellCols - 1; j++)
        {
            cells[i][j].clearCell();
        }
    }

    for (Ball* b : objects)
    {
       cells[b->position.y / cellHight + 1][b->position.x / cellWidth + 1].addBall(b);
    }
    size_t threadCount = 16;
    std::vector<std::thread> threads;
    /*for (int i = 1; i < cellRows - 1; i++)
    {
        for (int j = 1; j < cellCols - 1; j++)
        {
            if (cells[i][j].balls.empty())
                continue;
            for (int k = -1; k <= 1; k++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    if (cells[i + k][j + m].balls.empty())
                        continue;
                    collideCells(cells[i][j], cells[i + k][j + m]);
                }
            }
        }
    }*/

    //int rowsPerThread = (cellRows - 2) / threadCount;
    //int remainder = (cellRows - 2) % threadCount;

    int colsPerThread = (cellCols - 2) / threadCount;
    int remainder = (cellCols - 2) % threadCount;
    //int startRow = 1;
    int startCol = 1;
    for (int i = 0; i < threadCount; i++)
    {
        int endCol = startCol + colsPerThread + (i < remainder ? 1 : 0); // Добавляем оставшиеся строки, если нужно
        threads.emplace_back(processColsStatic, std::ref(cells), startCol, endCol);
        startCol = endCol; // Сдвигаем на следующий сегмент
        //std::cout << "Поток " << i << " " << (cellRows) / threadCount * i + 1 << " " << (cellRows) / threadCount * (i + 1) << '\n';
    }

    for (int i = 0; i <threadCount; i++)
    {
        threads[i].join();
        //std::cout << "Завершился поток " << i << std::endl;
    }
    
}

void BallCollideArena :: UpdatePositions(double dt)
{
    //#pragma omp parallel for
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
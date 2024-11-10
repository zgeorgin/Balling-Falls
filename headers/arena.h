#ifndef ARENA_H
#define ARENA_H
#include <physics.h>

class Arena
{
public:
    double delta = 1;
    //std::vector<std::shared_ptr<Body>> objects;
    int animationTicks = 0;
    virtual void ApplyForces() = 0;
    void AddObject(std::shared_ptr<Body> object) {};
    //Arena(std::vector<std::shared_ptr<Body>> vec) : objects(vec) {}
    //Arena() {}
};

using BallPtr = std::shared_ptr<Ball>;

class Cell
{
public:
	Cell(uint32_t u, uint32_t d, uint32_t l, uint32_t r) : ub(u), db(d), lb(l), rb(r){}
	std::vector<BallPtr> balls; //Каждый кадр получаем указатели на все шарики внутри клетки 
	uint32_t ub, db, lb, rb; //Границы
	bool inBorders(BallPtr ball); //Функция проверяет, внутри ли клетки шарик
	void fillCell(std::vector<BallPtr> allBalls); //Подаём все шарики в эту функцию, а она заполняет клетку нужными
	void collideWithCell(std::shared_ptr<Cell> another); //Просчитывает коллизии между шариками из себя и шариками из другой клетки
};


using CellPtr = std::shared_ptr<Cell>;
class BallCollideArena : public Arena
{
public:
    std::vector<std::vector<CellPtr>> cells;
    uint32_t uborder, dborder, rborder, lborder;
    std::vector<BallPtr> objects;
    virtual void ApplyForces() override;
    void ApplyGravity();
    void HandleCollisions();
    void UpdatePositions(double dt);
    void AddObject(BallPtr object) {objects.push_back(object);}
    BallCollideArena(std::vector<BallPtr> vec, uint32_t u, uint32_t d, uint32_t l, uint32_t r, size_t cellCountX, size_t cellCountY);
};

#endif
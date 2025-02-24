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
    Cell() : ub(0), lb(0), db(0), rb(0) {} 
    Cell(const Cell& another) : ub(another.ub), db(another.db), lb(another.lb), rb(another.rb) {}
	Cell(uint32_t u, uint32_t d, uint32_t l, uint32_t r) : ub(u), db(d), lb(l), rb(r){}
	std::vector<Ball*> balls; //Каждый кадр получаем указатели на все шарики внутри клетки 
	uint32_t ub, db, lb, rb; //Границы
	inline bool inBorders(Ball* ball); //Функция проверяет, внутри ли клетки шарик
	void fillCell(std::vector<Ball*> allBalls); //Подаём все шарики в эту функцию, а она заполняет клетку нужными (НЕ НАДО ТАК ДЕЛАТЬ!)
    void clearCell() {balls.clear();}
    void addBall(Ball* ball) {balls.push_back(ball);}
};

void collideCells(const Cell& cell1, const Cell& cell2);//Просчитывает коллизии между шариками из двух клеток

class BallCollideArena : public Arena
{
private:
    void proccessRows(size_t startRow, size_t endRow);
public:
    std::vector<std::vector<Cell>> cells;
    float cellWidth, cellHight;
    size_t cellRows, cellCols;
    uint32_t uborder, dborder, rborder, lborder;
    std::vector<Ball*> objects;
    virtual void ApplyForces() override;
    void ApplyGravity();
    void HandleCollisions();
    void UpdatePositions(double dt);
    void AddObject(Ball* object) {objects.push_back(object);}
    BallCollideArena(std::vector<Ball*> vec, uint32_t u, uint32_t d, uint32_t l, uint32_t r, size_t cellCountX, size_t cellCountY);
};

#endif
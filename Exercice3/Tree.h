#include <random>
#include <vector>
#include <iostream>

#define MAZE_WIDTH 9
#define MAZE_HEIGHT 5
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

#ifndef EXERCICE3_TREE_H
#define EXERCICE3_TREE_H

class Cell {
    friend class Tree;
public:
    Cell(int, int);
    int getX() const;
    int getY() const;

    friend std::ostream& operator<<(std::ostream&, const Cell&);

private:
    int x = -1, y = -1;
    int dir = -1;
    bool Ndir = false, Sdir = false, Edir = false, Wdir = false;
};

class Tree {
public:
    Tree();
    void initMaze();
    void GrowingTree();
    Cell* getCell(int, int);
    void display();

    friend std::ostream& operator<<(std::ostream&, const Tree&);

private:
    std::vector< std::vector<Cell*> > maze;
};

#endif

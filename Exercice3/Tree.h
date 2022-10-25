#include <random>
#include <vector>
#include <iostream>

#define MAZE_WIDTH 10
#define MAZE_HEIGHT 10
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

#ifndef EXERCICE3_TREE_H
#define EXERCICE3_TREE_H

class Cell {
    friend class Tree;

private:
    int x = -1, y = -1;

    int dir = -1;
    bool Ndir = false, Sdir = false, Edir = false, Wdir = false;

    bool target = false, source = false;
    int solutionIndex = 0;

    Cell* parent = nullptr;
    std::vector<Cell*> children;

    int g = 0;
    int h = 0;
    int f = 0;

public:
    Cell(int, int);
    int getX() const;
    int getY() const;

    friend std::ostream& operator<<(std::ostream&, const Cell&);
    void setTarget();
    void setSource();
    bool isTarget() const;
    bool isSource() const;

    int getSolutionIndex() const;
    void setSolutionIndex(int solutionIndex);

    Cell *getParent() const;
    void setParent(Cell *parent);

    const std::vector<Cell *> &getChildren() const;
    void addChild(Cell*);

    int getG() const;
    void setG(int g);
    int getH() const;
    void setH(int h);
    int getF() const;
    void setF(int f);
};

class Tree {
public:
    Tree();
    void initMaze();
    void GrowingTree();
    Cell* getCell(int, int);
    void display();

    Cell* getSource();
    Cell* getTarget();

    friend std::ostream& operator<<(std::ostream&, const Tree&);

private:
    std::vector< std::vector<Cell*> > maze;
};

#endif

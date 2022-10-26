#include <random>
#include <vector>
#include <iostream>
#include <map>
#include <thread>

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
    int solutionIndex = 0, visitedIndex = 0;

    Cell *parent = nullptr;
    std::vector<Cell *> children;

    int g = 0;
    int h = 0;
    int f = 0;

public:
    Cell(int, int);

    int getX() const;

    int getY() const;

    bool isNdir() const;

    bool isSdir() const;

    bool isEdir() const;

    bool isWdir() const;

    friend std::ostream &operator<<(std::ostream &, const Cell &);

    void setTarget();

    void setSource();

    bool isTarget() const;

    bool isSource() const;

    int getSolutionIndex() const;

    void setSolutionIndex(int solutionIndex);

    int getVisitedIndex() const;

    void setVisitedIndex(int visitedIndex);

    Cell *getParent() const;

    void setParent(Cell *parent);

    const std::vector<Cell *> &getChildren() const;

    void addChild(Cell *);

    int getG() const;

    void setG(int g);

    int getH() const;

    void setH(int h);

    int getF() const;

    void setF(int f);
};

class Tree {
public:
    Tree(int, int);

    int getHeight() const;

    int getWidth() const;

    void initMaze();

    void growingTree();

    Cell *getCell(int, int);

    void display(const std::string&);

    bool isThreeDigits();

    void resetTreeInformation(bool, bool);

    void generateSourceAndTarget();

    Cell *getSource();

    Cell *getTarget();

    std::map<int, int> dirX = {{NORTH, 0,},
                               {SOUTH, 0,},
                               {EAST,  1,},
                               {WEST,  -1,}};
    std::map<int, int> dirY = {{NORTH, -1,},
                               {SOUTH, 1,},
                               {EAST,  0,},
                               {WEST,  0,}};
    std::map<int, int> dirOpposite = {{NORTH, SOUTH,},
                                      {SOUTH, NORTH,},
                                      {EAST,  WEST,},
                                      {WEST,  EAST,}};

    friend std::ostream &operator<<(std::ostream &, const Tree &);

private:
    int height;
    int width;
    std::vector<std::vector<Cell *> > maze;
};

#endif

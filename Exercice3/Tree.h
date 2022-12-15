#pragma once

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

    bool vis = false;
    bool Ndir = false, Sdir = false, Edir = false, Wdir = false;

    bool target = false, source = false;
    int solutionIndex = 0, visitedIndex = 0;

    Cell *parent = nullptr;
    std::vector<Cell *> children;

    float g = 0;
    float h = 0;
    float f = 0;

public:
    Cell(int, int);

    int getX() const;

    int getY() const;

    bool isNdir() const;

    bool isSdir() const;

    bool isEdir() const;

    bool isWdir() const;

    void setTarget();

    void setSource();

    bool isTarget() const;

    void setSolutionIndex(int solutionIndex);

    int getVisitedIndex() const;

    void setVisitedIndex(int visitedIndex);

    Cell *getParent() const;

    void setParent(Cell *parent);

    const std::vector<Cell *> &getChildren() const;

    void addChild(Cell *);

    float getG() const;

    void setG(float g);

    float getH() const;

    void setH(float h);

    float getF() const;

    void setF(float f);
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

private:
    int height;
    int width;
    std::vector<std::vector<Cell *> > maze;
};

#endif

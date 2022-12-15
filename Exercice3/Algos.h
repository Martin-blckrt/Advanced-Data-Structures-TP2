#pragma once

#include <cmath>
#include <map>
#include <algorithm>
#include <queue>
#include <deque>
#include "Tree.h"

using namespace std;

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

// returns Manhattan distance between two points (i.e. grid distance without diagonal movements)
float getManhattanDistance(Cell* source, Cell* target) {

    float diffX = abs(source->getX() - target->getX());
    float diffY = abs(source->getY() - target->getY());
    return diffX + diffY;
}

// returns the Euclidean distance between two points (i.e. straight distance)
float getDistance(Cell* source, Cell* target) {

    int diffX = pow(source->getX() - target->getX(), 2);
    int diffY = pow(source->getY() - target->getY(), 2);
    return sqrt(diffX + diffY);
}

// check in the list if their exists a better cell (i.e. same position but obtained with less movements)
bool betterCell(Cell* child, vector<Cell*> &list) {

    for (auto elem: list)
        if (elem->getX() == child->getX() && elem->getY() == child->getY() && elem->getG() < child->getG())
            return true;

    return false;
}

// checks if a location is already explored
bool isCellUnvisited(int x, int y, vector<Cell*> &list) {

    for (auto elem: list)
        if (elem->getX() == x && elem->getY() == y)
            return false;

    return true;
}

// check if a direction is "legal" in the maze : it stays in bounds and there is no wall
bool isLegalMazeMove(Cell* current, int dir, Tree* tree) {

    auto newX = current->getX() + dirX[dir];
    auto newY = current->getY() + dirY[dir];
    if (newX >= 0 && newY >= 0 && newX < tree->getWidth() && newY < tree->getHeight()) {
        switch (dir) {
            case NORTH:
                if (current->isNdir() || tree->getCell(newX, newY)->isSdir())
                    return true;
                break;
            case SOUTH:
                if (current->isSdir() || tree->getCell(newX, newY)->isNdir())
                    return true;
                break;
            case EAST:
                if (current->isEdir() || tree->getCell(newX, newY)->isWdir())
                    return true;
                break;
            case WEST:
                if (current->isWdir() || tree->getCell(newX, newY)->isEdir())
                    return true;
                break;
            default:
                break;
        }
    }
    return false;
}

void buildCell(int x, int y, Cell* parentNode, Tree* tree) {

    Cell* childCell = tree->getCell(x, y);
    parentNode->addChild(childCell);
    childCell->setParent(parentNode);
}

// checks if a cell has not been explored yet and is a legal maze move before adding the cell
void expandCell(Cell* cell, vector<Cell*> &closed, Tree* tree) {

    int x = cell->getX(), y = cell->getY();

    if (isCellUnvisited(x - 1, y, closed) && isLegalMazeMove(cell, WEST, tree))
        buildCell(x - 1, y, cell, tree);

    if (isCellUnvisited(x + 1, y, closed) && isLegalMazeMove(cell, EAST, tree))
        buildCell(x + 1, y, cell, tree);

    if (isCellUnvisited(x, y - 1, closed) && isLegalMazeMove(cell, NORTH, tree))
        buildCell(x, y - 1, cell, tree);

    if (isCellUnvisited(x, y + 1, closed) && isLegalMazeMove(cell, SOUTH, tree))
        buildCell(x, y + 1, cell, tree);

}

// the uninformed search algorithm Breadth First Search
deque<Cell*> BFS(Tree* tree) {

    // reset information to get a "clear" maze with only source and target
    tree->resetTreeInformation(true, false);

    // this variable allows to store in which order the cells were explored by the algorithm
    int visIndex = 0;

    Cell* source = tree->getSource();

    vector <Cell*> visited;
    deque <Cell*> solution;
    queue <Cell*> qu;

    qu.push(source);

    // until we have explored every element in the queue
    while (!qu.empty()) {

        // get the first element
        Cell* current = qu.front();

        // mark it as explored in the visIndex's position
        current->setVisitedIndex(visIndex);

        // remove it from the queue
        qu.pop();

        // check if we have found the target
        if (current->isTarget()) {

            // build the solution from target and backtrack until source
            do {
                solution.insert(solution.begin(), current);
                current = current->getParent();
            } while (current->getParent() != nullptr);

            return solution;

        // if we have not found the target
        } else {

            // mark the cell as visited
            visited.push_back(current);

            // expand all possible cells around this cell
            expandCell(current, visited, tree);

            // add all expanded cells to the queue
            for (auto child: current->getChildren())
                qu.push(child);
        }
        tree->display("exploration");
        visIndex++;
    }
}

// the informed search algorithm A star
deque<Cell*> AStar(Tree* tree, bool heuristic) {

    // reset information to get a "clear" maze with only source and target
    tree->resetTreeInformation(true, false);

    // this variable allows to store in which order the cells were explored by the algorithm
    int visIndex = 0;

    Cell* source = tree->getSource();
    Cell* target = tree->getTarget();
    vector <Cell*> opened;
    vector <Cell*> closed;
    deque <Cell*> solution;

    opened.push_back(source);

    // until we have explored every element in the opened list
    while (!opened.empty()) {
        int index = 0, i = 0;
        Cell* current = opened[0];

        // get the element from the list with the smallest f
        for (auto elem: opened) {
            if (elem->getF() < current->getF()) {
                current = elem;
                index = i;
            }
            i++;
        }

        // remove that cell from the opened list
        opened.erase(opened.begin() + index);

        // add it to the closed list
        closed.push_back(current);

        // mark it as explored in the visIndex's position
        current->setVisitedIndex(visIndex);

        // check if we have found the target
        if (current->isTarget()) {

            // build the solution from target and backtrack until source
            do {
                solution.insert(solution.begin(), current);
                current = current->getParent();
            } while (current->getParent() != nullptr);

            return solution;

        }

        // expand all possible cells around this cell
        expandCell(current, closed, tree);

        // for every expanded cell, compute their updated g, h and f values
        for (auto child: current->getChildren()) {

            child->setG(current->getG() + 1);

            if (heuristic)
                child->setH(getDistance(child, target));
            else
                child->setH(getManhattanDistance(child, target));

            child->setF(child->getG() + child->getH());

            // if there exists a better cell than this child, don't add it to the opened list
            if (betterCell(child, opened))
                continue;

            // add this child to the opened list
            opened.push_back(child);
        }
        tree->display("exploration");
        visIndex++;
    }
}
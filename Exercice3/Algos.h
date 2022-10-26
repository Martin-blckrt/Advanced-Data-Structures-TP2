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

int getManhattanDistance(Cell* source, Cell* target) {

    int diffX = abs(source->getX() - target->getX());
    int diffY = abs(source->getY() - target->getY());
    return diffX + diffY;
}

bool betterCell(Cell* child, vector<Cell*> &list) {

    for (auto elem: list)
        if (elem->getX() == child->getX() && elem->getY() == child->getY() && elem->getG() < child->getG())
            return true;

    return false;
}

bool isCellUnvisited(int x, int y, vector<Cell*> &list) {

    for (auto elem: list)
        if (elem->getX() == x && elem->getY() == y)
            return false;

    return true;
}

bool isLegalMazeMove(Cell* current, int dir, Tree* tree) {

    auto newX = current->getX() + dirX[dir];
    auto newY = current->getY() + dirY[dir];
    if (newX >= 0 && newY >= 0 && newX < MAZE_WIDTH && newY < MAZE_HEIGHT) {
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

deque<Cell*> BFS(Tree* tree) {

    tree->resetTreeInformation(true, false);
    int visIndex = 0;

    Cell* source = tree->getSource();
    vector <Cell*> visited;
    deque <Cell*> solution;
    queue <Cell*> qu;

    qu.push(source);

    while (!qu.empty()) {
        Cell* current = qu.front();
        current->setVisitedIndex(visIndex);
        qu.pop();

        if (current->isTarget()) {

            do {
                solution.insert(solution.begin(), current);
                current = current->getParent();
            } while (current->getParent() != nullptr);

            return solution;

        } else {
            visited.push_back(current);
            expandCell(current, visited, tree);

            for (auto child: current->getChildren())
                qu.push(child);
        }
        tree->display("exploration");
        visIndex++;
    }
}

deque<Cell*> AStar(Tree* tree) {

    tree->resetTreeInformation(true, false);
    int visIndex = 0;

    Cell* source = tree->getSource();
    Cell* target = tree->getTarget();
    vector <Cell*> opened;
    vector <Cell*> closed;
    deque <Cell*> solution;

    opened.push_back(source);

    while (!opened.empty()) {
        int index = 0, i = 0;
        Cell* current = opened[0];

        for (auto elem: opened) {
            if (elem->getF() < current->getF()) {
                current = elem;
                index = i;
            }
            i++;
        }

        opened.erase(opened.begin() + index);
        closed.push_back(current);
        current->setVisitedIndex(visIndex);

        if (current->isTarget()) {

            do {
                solution.insert(solution.begin(), current);
                current = current->getParent();
            } while (current->getParent() != nullptr);

            return solution;

        }

        expandCell(current, closed, tree);

        for (auto child: current->getChildren()) {

            child->setG(current->getG() + 1);
            child->setH(getManhattanDistance(child, target));
            child->setF(child->getG() + child->getH());

            if (betterCell(child, opened))
                continue;

            opened.push_back(child);
        }
        tree->display("exploration");
        visIndex++;
    }
}
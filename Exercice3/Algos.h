#include <cmath>
#include <algorithm>
#include <queue>
#include <deque>
#include "Tree.h"

using namespace std;

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

void buildCell(int x, int y, Cell* parentNode, Tree* tree) {

    Cell* childCell = tree->getCell(x, y);
    parentNode->addChild(childCell);
    childCell->setParent(parentNode);
}

void expandCell(Cell* cell, vector<Cell*> &closed, Tree* tree) {

    int x = cell->getX(), y = cell->getY();

    if (x > 0 && isCellUnvisited(x - 1, y, closed))
        buildCell(x - 1, y, cell, tree);

    if (x < (MAZE_WIDTH - 1) && isCellUnvisited(x + 1, y, closed))
        buildCell(x + 1, y, cell, tree);

    if (y > 0 && isCellUnvisited(x, y - 1, closed))
        buildCell(x, y - 1, cell, tree);

    if (y < (MAZE_HEIGHT - 1) && isCellUnvisited(x, y + 1, closed))
        buildCell(x, y + 1, cell, tree);

}

deque<Cell*> BFS(Tree* tree) {

    Cell* source = tree->getSource();
    Cell* target = tree->getTarget();
    vector <Cell*> visited;
    deque <Cell*> solution;
    queue <Cell*> qu;

    qu.push(source);

    while (!qu.empty()) {
        Cell* current = qu.front();
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
    }
}

deque<Cell*> AStar(Tree* tree) {

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
    }
}
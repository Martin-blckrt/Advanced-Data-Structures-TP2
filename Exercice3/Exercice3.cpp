#include <iostream>
#include <random>
#include "Tree.h"
#include "Algos.h"

int main()
{
    srand(time(nullptr));
    auto tree = new Tree();
    tree->GrowingTree();
    tree->display();
    cout << "\n";
    auto xTarget = rand() % MAZE_WIDTH, yTarget = rand() % MAZE_HEIGHT;
    auto xSource = rand() % MAZE_WIDTH, ySource = rand() % MAZE_HEIGHT;
    while (xTarget == xSource && yTarget == ySource)
        xSource = rand() % MAZE_WIDTH, ySource = rand() % MAZE_HEIGHT;

    tree->getCell(xSource, ySource)->setSource();
    tree->getCell(xTarget, yTarget)->setTarget();
    cout << "Source is on : " << xSource << ", " << ySource << endl;
    cout << "Target is on : " << xTarget << ", " << yTarget << endl;
    tree->display();
    deque<Cell*> BFSSolution = AStar(tree);
    int i = 1;
    for (Cell* cell : BFSSolution) {
        cell->setSolutionIndex(i);
        i++;
    }
    tree->display();

}

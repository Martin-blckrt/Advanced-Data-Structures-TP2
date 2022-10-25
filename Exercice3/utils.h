#include <iostream>
#include <random>

using namespace std;

pair<int, int> generateSourceAndTarget(Tree* tree) {
    tree->resetTreeInformation();
    auto xTarget = rand() % MAZE_WIDTH, yTarget = rand() % MAZE_HEIGHT;
    auto xSource = rand() % MAZE_WIDTH, ySource = rand() % MAZE_HEIGHT;
    while (xTarget == xSource && yTarget == ySource)
        xSource = rand() % MAZE_WIDTH, ySource = rand() % MAZE_HEIGHT;

    tree->getCell(xSource, ySource)->setSource();
    tree->getCell(xTarget, yTarget)->setTarget();

    cout << "Source is on : " << xSource << ", " << ySource << endl;
    cout << "Target is on : " << xTarget << ", " << yTarget << endl;
}

void generateSolution(Tree* tree, string algo) {
    deque<Cell*> solution;
    if (algo == "BFS")
        solution = BFS(tree);
    else if (algo == "AStar")
        solution = AStar(tree);
    else
        cout << "Select a valid algorithm !" << endl;
    int i = 1;
    for (Cell* cell : solution) {
        cell->setSolutionIndex(i);
        i++;
    }
    tree->display();
}
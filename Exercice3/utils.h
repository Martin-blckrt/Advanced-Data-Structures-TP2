#include <iostream>

using namespace std;

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
    tree->display("finished");
    cout << algo << " found the solution in " << solution.back()->getVisitedIndex() << " iterations !" << endl;
}

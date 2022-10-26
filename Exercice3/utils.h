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

int getSize(string dim) {
    int newDim = 0;
    do {
        cout << "What " << dim << " do you want ? ";
        cin.clear();
        cin.ignore( numeric_limits<int>::max(), '\n' );
        cin >> newDim;
    } while (newDim < 2);

    return newDim;
}

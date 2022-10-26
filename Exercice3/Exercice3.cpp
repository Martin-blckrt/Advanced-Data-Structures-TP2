#include <iostream>
#include <random>
#include "Tree.h"
#include "Algos.h"
#include "utils.h"

void menu(Tree*);
void mainMenu();
int choice = 0;

int main() {
    srand(time(nullptr));
    auto tree = new Tree();
    menu(tree);
    return 0;
}

void menu(Tree* tree) {
    auto getTree = tree;
    do {
        mainMenu();

        switch(choice) {
            case 0:
                break;
            case 1:
                getTree = new Tree();
                break;
            case 2:
                getTree->display("empty");
                cout << "\n";
                break;
            case 3:
                generateSolution(getTree, "BFS");
                break;
            case 4:
                generateSolution(getTree, "AStar");
                break;
            case 5:
                getTree->generateSourceAndTarget();
                break;
            default:
                break;
        }
    } while(choice != 0);
}

void mainMenu() {
    cout << "\nSelect an action" << endl;
    cout << "1 - Change maze" << endl;
    cout << "2 - Print maze" << endl;
    cout << "3 - Solve maze using BFS" << endl;
    cout << "4 - Solve maze using A-star" << endl;
    cout << "5 - Generate new source and target" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice;
}

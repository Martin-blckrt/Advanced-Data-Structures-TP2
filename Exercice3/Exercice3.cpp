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
    auto tree = new Tree(5, 5);
    menu(tree);
    return 0;
}

void menu(Tree* tree) {
    auto getTree = tree;
    do {
        mainMenu();

        switch(choice) {
            case 1:
                getTree = new Tree(getTree->getHeight(), getTree->getWidth());
                break;
            case 2:
                getTree->generateSourceAndTarget();
                break;
            case 3:
                getTree = new Tree(getSize("height"), getSize("width"));
                break;
            case 4:
                getTree->display("empty");
                break;
            case 5:
                generateSolution(getTree, "BFS", false);
                break;
            case 6:
                bool h;
                do {
                    cout << "Which heuristic do you want to use ? Type 0 for Manhattan distance, 1 for Euclidian distance" << endl;
                    cin.clear();
                    cin.ignore( numeric_limits<int>::max(), '\n' );
                    cin >> h;
                } while (cin.fail());
                generateSolution(getTree, "AStar", h);
                break;
            default:
                break;
        }
    } while(choice != 0);
}

void mainMenu() {
    cout << "\nSelect an action" << endl;
    cout << "1 - Re-generate maze" << endl;
    cout << "2 - Generate new source and target" << endl;
    cout << "3 - Change maze dimensions (default is 5 by 5)" << endl;
    cout << "4 - Print maze" << endl;
    cout << "5 - Solve maze using BFS" << endl;
    cout << "6 - Solve maze using A-star" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice;
}

#include <iostream>
#include <random>
#include "Tree.h"
#include "Algos.h"
#include "utils.h"

void menu();
void mainMenu();
int choice = 0;

int main() {
    srand(time(nullptr));
    menu();
    return 0;
}

void menu() {
    do {

        auto tree = new Tree();
        tree->GrowingTree();
        mainMenu();

        switch(choice) {
            case 0:
                break;
            case 1:
                // change size and reset maze
                break;
            case 2:
                tree->display();
                cout << "\n";
                break;
            case 3:
                generateSourceAndTarget(tree);
                generateSolution(tree, "BFS");
                break;
            case 4:
                generateSourceAndTarget(tree);
                generateSolution(tree, "AStar");
                break;
            default:
                break;
        }
    } while(choice != 0);
}

void mainMenu() {
    cout << "\nSelect an action" << endl;
    cout << "1 - Change maze dimensions (default is 5 by 5)" << endl;
    cout << "2 - Print maze" << endl;
    cout << "3 - Solve maze using BFS" << endl;
    cout << "4 - Solve maze using A-star" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice;
}

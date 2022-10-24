// Exercice 4.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <Magick++.h>
#include <vector>
#include "utils.h"
#include "quadtree.h"
#include "Pixel.h"

using namespace Magick;
using namespace std;

void menu();
void mainMenu();
void optionsMenu();
void options(QuadTree<Pixel>*);
int choice1 = 0;
int choice2 = 5;

int main(int /*argc*/, char** argv)
{
    InitializeMagick(*argv);
    menu();
    return 0;
}

void menu() {

    QuadTree<Pixel>* qt;

    do {
        choice2 = 0;
        mainMenu();

        switch (choice1) {
        case 0:
            break;
        case 1:
            qt = getImageFromPath("Images/chat.png");
            options(qt);
            break;
        case 2:
            qt = getImageFromPath("Images/lune.png");
            options(qt);
            break;
        case 3:
            qt = getImageFromPath("Images/montgolfiere.png");
            options(qt);
            break;
        case 4:
            qt = getImageFromPath("Images/motif.png");
            options(qt);
            break;
        case 5:
            qt = getImageFromPath("Images/motif3.png");
            options(qt);
            break;
        case 6:
            qt = getImageFromPath("Images/soleil.png");
            options(qt);
            break;
        case 7:
            qt = getImageFromPath("Images/tableau.png");
            options(qt);
            break;
        case 8:
            qt = getImageFromPath("Images/tableau2.png");
            options(qt);
            break;
        default:
            break;
        }
    } while (choice1 != 0);
}

void options(QuadTree<Pixel>* qt) {
    do {
        optionsMenu();
        switch (choice2) {
        case 0:
            exit(0);
        case 1:
            display(qt);
            break;
        case 2:
            cout << qt->nNodes() << endl;
            break;
        case 3:
            cout << qt->nLeaves() << endl;
            break;
        case 4:
            cout << qt->nTrees() << endl;
            break;
        case 5:
            cout << "soon" << endl;
            break;
        case 6:
            break;
        default:
            break;
        }
    } while (choice2 != 6);
}

void mainMenu() {
    cout << "\n" << endl;
    cout << "Choose an image" << endl;
    cout << "1 - chat.png (225x225)" << endl;
    cout << "2 - lune.png (1024x1024)" << endl;
    cout << "3 - montgolfiere.png (225x225)" << endl;
    cout << "4 - motif.png (225x225)" << endl;
    cout << "5 - motif3.png (225x225)" << endl;
    cout << "6 - soleil.png (1024x1024)" << endl;
    cout << "7 - tableau.png (256x256)" << endl;
    cout << "8 - tableau2.png (256x256)" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice1;
}

void optionsMenu() {
    cout << "\n" << endl;
    cout << "Functions Menu" << endl;
    cout << "1 - Display quadtree" << endl;
    cout << "2 - Get number of nodes" << endl;
    cout << "3 - Get number of leaves" << endl;
    cout << "4 - Get number of trees" << endl;
    cout << "5 - Render image from quadtree" << endl;
    cout << "6 - Change image" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice2;
}
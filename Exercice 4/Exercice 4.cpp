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
void options(QuadTree<Pixel>*, size_t, string);
void recursiveImageFromTree(size_t, size_t, size_t, size_t, Image*, QuadTree<Pixel>*);
void recursiveImageFromTree(QuadTree<Pixel>*, size_t, size_t, string);

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
            options(qt, 255, "chat");
            break;
        case 2:
            qt = getImageFromPath("Images/montgolfiere.png");
            options(qt, 255, "montgolfiere");
            break;
        case 3:
            qt = getImageFromPath("Images/motif.png");
            options(qt, 255, "motif");
            break;
        case 4:
            qt = getImageFromPath("Images/motif3.png");
            options(qt, 255, "motif3");
            break;
        case 5:
            qt = getImageFromPath("Images/tableau.png");
            options(qt, 256, "tableau");
            break;
        case 6:
            qt = getImageFromPath("Images/tableau2.png");
            options(qt, 256, "tableau2");
            break;
        default:
            break;
        }
    } while (choice1 != 0);
}

void options(QuadTree<Pixel>* qt, size_t side, string img_name) {
    do {
        optionsMenu();
        switch (choice2) {
        case 0:
            exit(0);
        case 1:
            display(qt);
            break;
        case 2:
            cout << "Constructing image... Please wait" << endl;
            recursiveImageFromTree(qt, side, side, img_name);
            cout << "Image saved in /Compressed/ directory !" << endl;
            break;
        case 3:
            break;
        default:
            break;
        }
    } while (choice2 != 3);
}

void mainMenu() {
    cout << "\n" << endl;
    cout << "Choose an image" << endl;
    cout << "1 - chat.png (225x225)" << endl;
    cout << "2 - montgolfiere.png (225x225)" << endl;
    cout << "3 - motif.png (225x225)" << endl;
    cout << "4 - motif3.png (225x225)" << endl;
    cout << "5 - tableau.png (256x256)" << endl;
    cout << "6 - tableau2.png (256x256)" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice1;
}

void optionsMenu() {
    cout << "\n" << endl;
    cout << "Functions Menu" << endl;
    cout << "1 - Display quadtree (/!\\ lot of prints /!\\)" << endl;
    cout << "2 - Render image from quadtree" << endl;
    cout << "3 - Change image" << endl;
    cout << "0 - Exit" << endl;
    cout << "Please choose: ";
    cin >> choice2;
}


/**
 * Construit récursivement une image à partir d'un QuadTree
 *
 * @param *qt Pointeur vers le QuadTree<Pixel> à parcourir
 * @param height, width Taille de l'image
 * @param img_name Nom de l'image à sauvegarder
 *
 * @return _ Sauvegarde l'image construite dans ./Compressed/
 */
void recursiveImageFromTree(QuadTree<Pixel> *qt, size_t height, size_t width, string img_name)
{
    Image image(Geometry(width, height), "white");
    image.type(TrueColorAlphaType);
    image.modifyImage();
    recursiveImageFromTree(width, height, 0, 0, &image, qt);
    image.write("./Compressed/" + img_name + ".png");
}

/**
 * Construit récursivement une image à partir d'un QuadTree
 *
 * @param *qt Pointeur vers le QuadTree<Pixel> à parcourir
 * @param height, width Taille de l'image
 * @param img_name Nom de l'image à sauvegarder
 *
 * @return _ Sauvegarde l'image construite dans ./Compressed/
 */
void recursiveImageFromTree(size_t height, size_t width, size_t i, size_t j, Image* img, QuadTree<Pixel>* qt)
{
    if (qt->isLeaf())
    {
        //get the color of the leaf
        Pixel leafPixel = qt->value();
        double red = leafPixel.getRed();
        double green = leafPixel.getGreen();
        double blue = leafPixel.getBlue();
        //visit all the pixels covered by the leaf
        for (size_t row = i; row < i + height; row++)
        {
            for (size_t col = j; col < j + width; col++)
            {
                ColorRGB px = ColorRGB(red, green, blue);
                //change their color to the color of the leaf
                img->pixelColor(col, row, px);
            }
        }
        //save changes
        img->syncPixels();
        return;
    }
    size_t halfHeight = 0;
    size_t halfWidth = 0;
    int heightAdjustment = 0;
    int widthAdjustment = 0;

    if (height != 1)
    {
        halfHeight = height / 2;
        heightAdjustment = (height % 2 == 0) ? 0 : 1;     //Si height impair
    }
    else
    {
        halfHeight = height;
    }

    if (width != 1)
    {
        halfWidth = width / 2;
        widthAdjustment = (width % 2 == 0) ? 0 : 1;     //Si width impair
    }
    else
    {
        halfWidth = width;

    }
    //Generate image from the 4 subQuadTree
    recursiveImageFromTree(halfHeight + heightAdjustment, halfWidth, i + halfHeight, j, img, qt->son(0)); //SO
    recursiveImageFromTree(halfHeight, halfWidth, i, j, img, qt->son(1)); //NO
    recursiveImageFromTree(halfHeight, halfWidth + widthAdjustment, i, j + halfWidth, img, qt->son(2)); //NE
    recursiveImageFromTree(halfHeight + heightAdjustment, halfWidth + widthAdjustment, i + halfHeight, j + halfWidth, img, qt->son(3)); //SE

}
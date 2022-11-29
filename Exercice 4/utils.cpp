#include <Magick++.h>
#include "utils.h"
#include "quadtree.h"

using namespace std;
using namespace Magick;

/**
 * Transfert les pixels d'une image en vecteur 2D
 *
 * @param path Chemin relatif ou complet vers l'image.
 * 
 * @return QuadTree<Pixel>* Après l'appel récursif vers constructTree.
 */
QuadTree<Pixel>* getImageFromPath(string const path)
{
    cout << "Transforming image into quadtree please wait . . . (10 to 15 seconds)" << endl;
    Image image;

    image.read(path); // Lit l'image au chemin donné
    
    // Transfère Pixel(0,0) au cache de pixels
    Quantum* pixels = image.getPixels(0, 0, image.columns(), image.rows());

    // Vecteur 2D de même taille que l'image actuelle
    vector < vector < Pixel > > matrix(image.rows(), vector < Pixel >(image.columns(), Pixel()));

    // On remplit le vecteur
    for (auto row = 0; row < image.rows(); row++)
    {
        for (auto column = 0; column < image.columns(); column++)
        {
            ColorRGB pix = image.pixelColor(column, row); // récupération du pixel actuel
            matrix[row][column] = Pixel(pix.red(), pix.green(), pix.blue()); // sauvegarde avec custom class Pixel
        }
    }
    return constructTree(matrix.size(), matrix[0].size(), 0, 0, matrix); // (height, width, x, y, 2D Vector)
}

/**
 * Construit récursivement un QuadTree à partir d'un vecteur 2D
 *
 * @param height, width : Taille du morceau actuel
 * @param x, y : Position actuelle dans l'image
 * @param img : Vecteur 2D à parcourir 
 * 
 * @return QuadTree<Pixel>* vers getImageFromPath pour retour dans Exercice4.cpp
 */
QuadTree<Pixel>* constructTree(size_t height, size_t width, size_t x, size_t y, vector<vector<Pixel>> const img)
{   
    //Si image simple : 1 pixel (feuille)
    if (height == 1 && width == 1)
    {
        return new QuadLeaf<Pixel>(img[x][y]);
    }
    //Sinon on découpe l'image en 4 morceaux de même taille
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

    QuadTree<Pixel>* sw = constructTree(halfHeight + heightAdjustment, halfWidth, x + halfHeight, y, img);
    QuadTree<Pixel>* nw = constructTree(halfHeight, halfWidth, x, y, img);
    QuadTree<Pixel>* ne = constructTree(halfHeight, halfWidth + widthAdjustment, x, y + halfWidth, img);
    QuadTree<Pixel>* se = constructTree(halfHeight + heightAdjustment, halfWidth + widthAdjustment, x + halfHeight, y + halfWidth, img);

    // Si 4 Quadtree correspondant à l'encodage de 4 sous-régions ont la même couleur
    // On retourne une feuille de même couleur (inutile de construire 4 feuilles similaires)
    if (sw->isLeaf() && nw->isLeaf() && ne->isLeaf() && se->isLeaf() &&
        sw->value() == nw->value() &&
        sw->value() == ne->value() &&
        sw->value() == se->value())
    {
        return new QuadLeaf<Pixel>(sw->value()); // Feuille 
    }

    // Sinon on retourne un nouveau noeud avec ces 4 Quadtree comme père
    return new QuadNode<Pixel>(sw, nw, ne, se);
}
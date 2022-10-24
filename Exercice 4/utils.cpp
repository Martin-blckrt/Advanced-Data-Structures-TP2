#include <Magick++.h>
#include "utils.h"
#include "quadtree.h"

using namespace std;
using namespace Magick;

QuadTree<Pixel>* getImageFromPath(string const path)
{
    cout << "Transforming image into quadtree please wait . . ." << endl;
    Image image;

    // Read a file into image object 
    image.read(path);
    image.modifyImage();
    
    Quantum* pixels = image.getPixels(0, 0, image.columns(), image.rows());
    vector < vector < Pixel > > matrix(image.rows(), vector < Pixel >(image.columns(), Pixel()));

    for (auto row = 0; row < image.rows(); row++)
    {
        for (auto column = 0; column < image.columns(); column++)
        {
            ColorRGB pix = image.pixelColor(column, row);
            matrix[row][column] = Pixel(pix.red(), pix.green(), pix.blue());
        }
    }
    return constructTree(matrix.size(), matrix[0].size(), 0, 0, matrix);
}


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

    // Sinon on retourne un nouveau noeud avec ces 4 Quadtree comme fils
    return new QuadNode<Pixel>(sw, nw, ne, se);
}

/*vector<vector<Pixel>> contructImageFromTree(QuadTree<Pixel>)
{
}*/
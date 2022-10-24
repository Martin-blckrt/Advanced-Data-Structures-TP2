#include <Magick++.h>
#include "utils.h"
#include "quadtree.h"

using namespace std;
using namespace Magick;

vector<vector<Pixel>> getImageFromPath(string const path)
{
    Image image;

    // Read a file into image object 
    image.read(path);
    image.modifyImage();

    cout << image.columns() << endl;
    cout << image.rows() << endl;

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
    return matrix;
}

QuadTree<Pixel> constructTree();
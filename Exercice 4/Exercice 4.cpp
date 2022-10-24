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


int main(int /*argc*/, char** argv)
{
    InitializeMagick(*argv);
       
    try {
        auto mat = getImageFromPath("Images/chat.png");
        //auto tree = constructTree(mat);
    }
    catch (Exception& error_)
    {
        cout << "Caught exception: " << error_.what() << endl;
        return EXIT_FAILURE;
    }
    
    /*for (const auto& row : matrix)
    {
        for (const auto& s : row)
        {
            cout << s << ' ';
        }
        cout << endl;
    }*/

    return 0;
}

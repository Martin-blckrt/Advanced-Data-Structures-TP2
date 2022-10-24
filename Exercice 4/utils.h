#pragma once

#include <vector>
#include "Pixel.h"
#include "quadtree.h"

using namespace std;

vector<vector<Pixel>> getImageFromPath(string const);
QuadTree<Pixel> constructTree();

#pragma once

#include <vector>
#include "Pixel.h"
#include "quadtree.h"

using namespace std;

QuadTree<Pixel>* getImageFromPath(string const);
QuadTree<Pixel>* constructTree(size_t, size_t, size_t, size_t, vector<vector<Pixel>> const);
vector<vector<Pixel>> contructImageFromTree(QuadTree<Pixel>);
#include <iostream>
#include <random>
#include "Tree.h"

int main()
{
    srand(time(nullptr));
    auto tree = new Tree();
    tree->GrowingTree();
}

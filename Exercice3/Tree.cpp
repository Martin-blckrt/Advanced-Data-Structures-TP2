#include <chrono>
#include <algorithm>
#include "Tree.h"

using namespace std;

Tree::Tree(int h, int w) {
    height = h;
    width = w;
    initMaze();
    growingTree();
    generateSourceAndTarget();
}

Cell* Tree::getCell(int x, int y) {
    return maze[x][y];
}

void Tree::initMaze() {
    for (int i = 0; i < width; i++) {
        vector<Cell *> v;
        for (int j = 0; j < height; j++)
            v.push_back(new Cell(i, j));
        maze.push_back(v);
    }
}

void Tree::growingTree() {
    unsigned seed = chrono::system_clock::now()
            .time_since_epoch()
            .count();

    auto x = rand() % width;
    auto y = rand() % height;

    vector<Cell *> list;
    vector<int> directions = {NORTH, SOUTH, EAST, WEST};

    list.push_back(maze[x][y]);

    while (!list.empty()) {
        // random select in list
        shuffle(list.begin(), list.end(), default_random_engine(seed));
        Cell *currentCell = list.front();
        bool index = true;
        x = currentCell->getX();
        y = currentCell->getY();

        shuffle(directions.begin(), directions.end(), default_random_engine(seed));

        for (int dir : directions) {
            auto newX = x + dirX[dir];
            auto newY = y + dirY[dir];
            if (newX >= 0 && newY >= 0 && newX < width && newY < height && maze[newX][newY]->dir == -1) {
                maze[x][y]->dir = dir;

                switch (dir) {
                    case NORTH:
                        maze[x][y]->Ndir = true;
                        break;
                    case SOUTH:
                        maze[x][y]->Sdir = true;
                        break;
                    case EAST:
                        maze[x][y]->Edir = true;
                        break;
                    case WEST:
                        maze[x][y]->Wdir = true;
                        break;
                    default:
                        break;
                }

                maze[newX][newY]->dir = dirOpposite[dir];
                list.push_back(maze[newX][newY]);
                index = false;

                break;
            }
        }
        if (index)
            list.erase(list.begin());
    }
}

bool Tree ::isThreeDigits() {
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            if (maze[i][j]->visitedIndex > 9)
                return true;
    return false;
}
/*
void Tree::display(const string& mode) {
    if (isThreeDigits() && mode == "exploration")
        displayThreeDigits();
    else {
        cout << endl;
        for (int k = 0; k < width*2; k++)
            cout << "# ";
        cout << "# " << endl;
        for (int i = 0; i < height*2; i++) {
            cout << "# ";
            for (int j = 0; j < width*2; j++) {
                int x = i/2;
                int y = j/2;
                if (i % 2 == 0 && j % 2 == 0) { // Info de cellule

                    if (mode == "empty") {
                        cout << ". ";
                    } else if (mode == "finished") {
                        if (this->maze[y][x]->source)
                            cout << "S ";
                        else if (this->maze[y][x]->target)
                            cout << "T ";
                        else if (this->maze[y][x]->solutionIndex != 0)
                            cout << "* ";
                        else
                            cout << ". ";
                    } else if (mode == "exploration") {
                        if (this->maze[y][x]->source)
                            cout << "S ";
                        else if (this->maze[y][x]->target)
                            cout << "T ";
                        else if (this->maze[y][x]->visitedIndex != 0)
                            cout << this->maze[y][x]->visitedIndex << " ";
                        else
                            cout << ". ";
                    }

                } else if (i % 2 == 1 && j % 2 == 0) {  // Info de mur vertical

                    if (i == (height*2 - 1)) // Si on est la dernière ligne
                        cout << "# ";
                    else {
                        if (this->maze[y][x]->Sdir || this->maze[y][x + 1]->Ndir)
                            cout << "  ";
                        else
                            cout << "# ";
                    }

                } else if (i % 2 == 0 && j % 2 == 1) {  // Info de mur horizontal

                    if (j == (width*2 - 1)) // Si on est la dernière colonne
                        cout << "# ";
                    else {
                        if (this->maze[y][x]->Edir || this->maze[y + 1][x]->Wdir)
                            cout << "  ";
                        else
                            cout << "# ";
                    }

                } else  // Info d'intersection
                    cout << "# ";
            }
            cout << endl;
        }
        if (mode == "exploration")
            this_thread::sleep_for(1000ms);
    }
}
*/
void Tree::display(const string& mode) {
    cout << endl;
    for (int k = 0; k < width*2; k++)
        cout << "## ";
    cout << "##" << endl;
    for (int i = 0; i < height*2; i++) {
        cout << "## ";
        for (int j = 0; j < width * 2; j++) {
            int x = i / 2;
            int y = j / 2;
            if (i % 2 == 0 && j % 2 == 0) { // Info de cellule

                if (mode == "empty") {
                    cout << " . ";
                } else if (mode == "finished") {
                    if (this->maze[y][x]->source)
                        cout << " S ";
                    else if (this->maze[y][x]->target)
                        cout << " T ";
                    else if (this->maze[y][x]->solutionIndex != 0)
                        cout << " * ";
                    else
                        cout << " . ";
                } else if (mode == "exploration") {
                    if (this->maze[y][x]->source)
                        cout << " S ";
                    else if (this->maze[y][x]->target)
                        cout << " T ";
                    else if (this->maze[y][x]->visitedIndex != 0 && this->maze[y][x]->visitedIndex < 10)
                        cout << " " << this->maze[y][x]->visitedIndex << " ";
                    else if (this->maze[y][x]->visitedIndex >= 10)
                        cout << this->maze[y][x]->visitedIndex << " ";
                    else
                        cout << " . ";
                }

            } else if (i % 2 == 1 && j % 2 == 0) {  // Info de mur vertical

                if (i == (height * 2 - 1)) // Si on est la dernière ligne
                    cout << "## ";
                else {
                    if (this->maze[y][x]->Sdir || this->maze[y][x + 1]->Ndir)
                        cout << "   ";
                    else
                        cout << "## ";
                }

            } else if (i % 2 == 0 && j % 2 == 1) {  // Info de mur horizontal

                if (j == (width * 2 - 1)) // Si on est la dernière colonne
                    cout << "## ";
                else {
                    if (this->maze[y][x]->Edir || this->maze[y + 1][x]->Wdir)
                        cout << "   ";
                    else
                        cout << "## ";
                }

            } else  // Info d'intersection
                cout << "## ";
        }
        cout << endl;
    }
    if (mode == "exploration")
        this_thread::sleep_for(1000ms);

}

Cell* Tree::getTarget() {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if(maze[j][i]->target)
                return maze[j][i];
    return nullptr;
}

Cell* Tree::getSource() {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if(maze[j][i]->source)
                return maze[j][i];
    return nullptr;
}

void Tree :: resetTreeInformation(bool solution, bool sourceAndTarget) {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            if (solution) {
                maze[j][i]->solutionIndex = 0;
                maze[j][i]->visitedIndex = 0;
                maze[j][i]->parent = nullptr;
                maze[j][i]->children.clear();
                maze[j][i]->g = 0;
                maze[j][i]->h = 0;
                maze[j][i]->f = 0;
            }
            if (sourceAndTarget) {
                maze[j][i]->source = false;
                maze[j][i]->target = false;
            }
        }
}

void Tree :: generateSourceAndTarget() {

    resetTreeInformation(false, true);
    auto xTarget = rand() % width, yTarget = rand() % height;
    auto xSource = rand() % width, ySource = rand() % height;
    while (xTarget == xSource && yTarget == ySource)
        xSource = rand() % width, ySource = rand() % height;

    maze[xSource][ySource]->setSource();
    maze[xTarget][yTarget]->setTarget();
}


ostream &operator<<(ostream &output, const Tree &t) {

    for (auto &i: t.maze) {
        output << "|";
        for (auto &j: i)
            output << *j << " ";
        output << endl;
    }
    return output;
}

int Tree::getHeight() const {
    return height;
}

int Tree::getWidth() const {
    return width;
}

ostream &operator<<(ostream &output, const Cell &c) {

    string res;

    if (res.empty())
        res = ".";

    res.append(" ");

    output << res;

    return output;
}

Cell::Cell(int coordX, int coordY) {
    x = coordX;
    y = coordY;
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}

void Cell::setTarget() {
    Cell::target = true;
}

void Cell::setSource() {
    Cell::source = true;
}

bool Cell::isTarget() const {
    return target;
}

bool Cell::isSource() const {
    return source;
}

Cell *Cell::getParent() const {
    return parent;
}

void Cell::setParent(Cell *parent) {
    Cell::parent = parent;
}

const vector<Cell *> &Cell::getChildren() const {
    return children;
}

void Cell::addChild(Cell* child) {
    Cell::children.push_back(child);
}

int Cell::getG() const {
    return g;
}

void Cell::setG(int g) {
    Cell::g = g;
}

int Cell::getH() const {
    return h;
}

void Cell::setH(int h) {
    Cell::h = h;
}

int Cell::getF() const {
    return f;
}

void Cell::setF(int f) {
    Cell::f = f;
}

int Cell::getSolutionIndex() const {
    return solutionIndex;
}

void Cell::setSolutionIndex(int solutionIndex) {
    Cell::solutionIndex = solutionIndex;
}

bool Cell::isNdir() const {
    return Ndir;
}

bool Cell::isSdir() const {
    return Sdir;
}

bool Cell::isEdir() const {
    return Edir;
}

bool Cell::isWdir() const {
    return Wdir;
}

int Cell::getVisitedIndex() const {
    return visitedIndex;
}

void Cell::setVisitedIndex(int visitedIndex) {
    Cell::visitedIndex = visitedIndex;
}

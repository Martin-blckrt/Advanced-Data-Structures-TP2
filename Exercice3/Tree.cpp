#include <chrono>
#include <algorithm>
#include <map>
#include "Tree.h"

using namespace std;

Tree::Tree() {
    initMaze();
}

Cell* Tree::getCell(int x, int y) {
    return maze[x][y];
}

void Tree::initMaze() {
    for (int i = 0; i < MAZE_WIDTH; i++) {
        vector<Cell *> v;
        for (int j = 0; j < MAZE_HEIGHT; j++)
            v.push_back(new Cell(i, j));
        maze.push_back(v);
    }
}


void Tree::GrowingTree() {
    unsigned seed = chrono::system_clock::now()
            .time_since_epoch()
            .count();

    map<int, int> dirX = {{NORTH, 0,},
                          {SOUTH, 0,},
                          {EAST,  1,},
                          {WEST,  -1,}};
    map<int, int> dirY = {{NORTH, -1,},
                          {SOUTH, 1,},
                          {EAST,  0,},
                          {WEST,  0,}};
    map<int, int> dirOpposite = {{NORTH, SOUTH,},
                                 {SOUTH, NORTH,},
                                 {EAST,  WEST,},
                                 {WEST,  EAST,}};

    auto x = rand() % MAZE_WIDTH;
    auto y = rand() % MAZE_HEIGHT;

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
            if (newX >= 0 && newY >= 0 && newX < MAZE_WIDTH && newY < MAZE_HEIGHT && maze[newX][newY]->dir == -1) {
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
/*
void Tree::display() {
    for (int k = 0; k < MAZE_WIDTH; k++)
        cout << "._";
    cout << "." << endl;
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (i + 1 < MAZE_HEIGHT) {
                if (this->maze[j][i]->Sdir || this->maze[j][i + 1]->Ndir)
                    cout << " ";
                else
                    cout << "_";
            } else
                cout << "_";

            if (j + 1 < MAZE_WIDTH) {
                if (this->maze[j][i]->Edir || this->maze[j + 1][i]->Wdir)
                    cout << " ";
                else
                    cout << "|";
            } else
                cout << "|";
        }
        cout << endl;
    }
}
*/

void Tree::display() {
    for (int k = 0; k < MAZE_WIDTH; k++)
        cout << "._";
    cout << "." << endl;
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (i + 1 < MAZE_HEIGHT) {
                if (this->maze[j][i]->Sdir || this->maze[j][i + 1]->Ndir) {
                    if (this->maze[j][i]->source)
                        cout << "S";
                    else if (this->maze[j][i]->target)
                        cout << "T";
                    else if (this->maze[j][i]->solutionIndex != 0)
                        cout << this->maze[j][i]->solutionIndex;
                    else
                        cout << " ";
                } else
                    cout << "_";
            } else
                cout << "_";

            if (j + 1 < MAZE_WIDTH) {
                if (this->maze[j][i]->Edir || this->maze[j + 1][i]->Wdir) {
                    if (this->maze[j][i]->source)
                        cout << "S";
                    else if (this->maze[j][i]->target)
                        cout << "T";
                    else if (this->maze[j][i]->solutionIndex != 0)
                        cout << this->maze[j][i]->solutionIndex;
                    else
                        cout << " ";
                } else
                    cout << "|";
            } else
                cout << "|";
        }
        cout << endl;
    }
}

Cell* Tree::getTarget() {
    for (int i = 0; i < MAZE_HEIGHT; i++)
        for (int j = 0; j < MAZE_WIDTH; j++)
            if(maze[i][j]->target)
                return maze[i][j];
    return nullptr;
}

Cell* Tree::getSource() {
    for (int i = 0; i < MAZE_HEIGHT; i++)
        for (int j = 0; j < MAZE_WIDTH; j++)
            if(maze[i][j]->source)
                return maze[i][j];
    return nullptr;
}

void Tree :: resetTreeInformation() {
    for (int i = 0; i < MAZE_HEIGHT; i++)
        for (int j = 0; j < MAZE_WIDTH; j++) {
            maze[i][j]->source = false;
            maze[i][j]->target = false;
            maze[i][j]->solutionIndex = 0;
            maze[i][j]->parent = nullptr;
            maze[i][j]->children.clear();
            maze[i][j]->g = 0;
            maze[i][j]->h = 0;
            maze[i][j]->f = 0;
        }
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

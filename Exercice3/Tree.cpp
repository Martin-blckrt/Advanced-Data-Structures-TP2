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

// Initialize a maze of defined height and width with empty cells
void Tree::initMaze() {
    for (int i = 0; i < width; i++) {
        vector<Cell *> v;
        for (int j = 0; j < height; j++)
            v.push_back(new Cell(i, j));
        maze.push_back(v);
    }
}
// The growing tree algorithm used to create a maze
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

        // randomly select a cell in the list
        shuffle(list.begin(), list.end(), default_random_engine(seed));
        Cell *currentCell = list.front();

        bool index = true;
        x = currentCell->getX();
        y = currentCell->getY();

        // shuffle the directions vector to get a random direction to explore
        shuffle(directions.begin(), directions.end(), default_random_engine(seed));

        for (int dir : directions) {

            // if dir is a valid direction, we will break out of the for loop
            auto newX = x + dirX[dir];
            auto newY = y + dirY[dir];

            // if new cell is valid : in the maze and unexplored
            if (newX >= 0 && newY >= 0 && newX < width && newY < height && maze[newX][newY]->dir == -1) {

                // set this cell as explored
                maze[x][y]->dir = dir;

                // set the new cell as explored
                maze[newX][newY]->dir = dirOpposite[dir];

                // update this cell to keep the direction in which it "carved" a passage in the maze
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

                // add the new cell to the list
                list.push_back(maze[newX][newY]);

                // set index to false so we do not erase the cell from the list : we havent finished exploring around it
                index = false;

                break;
            }
        }
        // if index is true, then the cell we just explored failed for all 4 directions : it is a "dead" cell
        if (index)
            list.erase(list.begin());
    }
}

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
            if (i % 2 == 0 && j % 2 == 0) { // Cell information

                // empty mode prints only the maze and its walls
                if (mode == "empty") {
                    cout << " . ";

                // finished mode prints the path between a source and a target
                } else if (mode == "finished") {
                    if (this->maze[y][x]->source)
                        cout << " S ";
                    else if (this->maze[y][x]->target)
                        cout << " T ";
                    else if (this->maze[y][x]->solutionIndex != 0)
                        cout << " * ";
                    else
                        cout << " . ";

                // exploration mode prints each step of an algorithm
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

            } else if (i % 2 == 1 && j % 2 == 0) {  // Vertical walls information

                if (i == (height * 2 - 1)) // If we are on last line
                    cout << "## ";
                else {
                    if (this->maze[y][x]->Sdir || this->maze[y][x + 1]->Ndir)
                        cout << "   ";
                    else
                        cout << "## ";
                }

            } else if (i % 2 == 0 && j % 2 == 1) {  // Horizontal walls information

                if (j == (width * 2 - 1)) // If we are on last column
                    cout << "## ";
                else {
                    if (this->maze[y][x]->Edir || this->maze[y + 1][x]->Wdir)
                        cout << "   ";
                    else
                        cout << "## ";
                }

            } else  // Intersections between walls, always a wall
                cout << "## ";
        }
        cout << endl;
    }
    // Mode called while printing maze exploration, sleep for clarity
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

// resets tree information, etiher only source and target, only an algorithm exploration, or both
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

// generates new source and target, checks for them to be different
void Tree :: generateSourceAndTarget() {

    resetTreeInformation(false, true);
    auto xTarget = rand() % width, yTarget = rand() % height;
    auto xSource = rand() % width, ySource = rand() % height;
    while (xTarget == xSource && yTarget == ySource)
        xSource = rand() % width, ySource = rand() % height;

    maze[xSource][ySource]->setSource();
    maze[xTarget][yTarget]->setTarget();
}

int Tree::getHeight() const {
    return height;
}

int Tree::getWidth() const {
    return width;
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

Cell *Cell::getParent() const {
    return parent;
}

void Cell::setParent(Cell *newParent) {
    Cell::parent = newParent;
}

const vector<Cell *> &Cell::getChildren() const {
    return children;
}

void Cell::addChild(Cell* child) {
    Cell::children.push_back(child);
}

float Cell::getG() const {
    return g;
}

void Cell::setG(float newG) {
    Cell::g = newG;
}

float Cell::getH() const {
    return h;
}

void Cell::setH(float newH) {
    Cell::h = newH;
}

float Cell::getF() const {
    return f;
}

void Cell::setF(float newF) {
    Cell::f = newF;
}

void Cell::setSolutionIndex(int solIndex) {
    Cell::solutionIndex = solIndex;
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

void Cell::setVisitedIndex(int visIndex) {
    Cell::visitedIndex = visIndex;
}

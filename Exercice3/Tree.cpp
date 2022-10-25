#include <chrono>
#include <algorithm>
#include <map>
#include "Tree.h"

using namespace std;

Tree::Tree() {
    initMaze();
}

Cell *Tree::getCell(int x, int y) {
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

    display();

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

                display();
                cout << "\n\n";
                break;
            }
        }
        if (index)
            list.erase(list.begin());
    }
}

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

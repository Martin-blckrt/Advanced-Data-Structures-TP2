#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

class Point {
public:
	int x;
    int y;

	Point(int pointX,int pointY) : x(pointX), y(pointY) {}

};

class KdNode {
public:
    Point* p;
	KdNode* gauche;
    KdNode* droit;

	KdNode(Point* point,KdNode* nodeG = nullptr,KdNode* nodeD = nullptr) : p(point), gauche(nodeG),droit(nodeD) {}

};

class KdTree {
    public:

    inline explicit KdTree();
	inline ~KdTree();

    inline void ajouterNoeud(KdNode* noeud);
    inline void supprimerNoeud(KdNode* noeud);
    inline KdNode* chercherNoeud(Point* point);
    inline void plusProcheVoisin();

    private:
        KdNode* racine;

};
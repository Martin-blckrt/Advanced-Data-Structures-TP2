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
    int hauteur;

	KdNode(Point* point,KdNode* nodeG = nullptr,KdNode* nodeD = nullptr) : p(point), gauche(nodeG),droit(nodeD) {}

};

class KdTree { // comparaison commence en X puis Y,X,Y ect...
    public:

    inline explicit KdTree();
	inline ~KdTree();

    inline void ajouterNoeud(KdNode* noeud);
    inline void supprimerNoeud(KdNode* noeud);
    inline KdNode* chercherNoeud(Point* point);
    inline void plusProcheVoisin();

    private:
        
    KdNode* racine;
    inline void _detruire(KdNode*);
    inline void _ajouterNoeud(KdNode*,KdNode*);

};

KdTree::KdTree(){
    racine = nullptr;
}

KdTree::~KdTree(){
    _detruire(racine);
}

void KdTree::_detruire(KdNode* elem){
    if(elem!=nullptr){
        _detruire(elem->droit);
        _detruire(elem->droit);
        delete elem;
    }
}

void KdTree::ajouterNoeud(KdNode* node){
    _ajouterNoeud(racine,node);
}

void KdTree::_ajouterNoeud(KdNode* nodeCompa,KdNode* nodeAjout){
    
}

void KdTree::supprimerNoeud(KdNode* node){
    
}
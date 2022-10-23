#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

class Point {
public:
    string nom;
	int x;
    int y;

	Point(string nomP,int pointX,int pointY) : nom(nomP), x(pointX), y(pointY) {}

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
    inline void afficherArbre();

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
    if(nodeCompa == nullptr){ // cas racine
    racine = nodeAjout;
    racine->hauteur = 0;
    }
    if(nodeCompa->hauteur % 2 == 0){ // comparaison sur X
        if(nodeAjout->p->x < nodeCompa->p->x){ // à gauche
            if(nodeCompa->gauche == nullptr){ // aucun élément à gauche, ajout du nouvel élément
                nodeCompa->gauche = nodeAjout;
                nodeAjout->hauteur = nodeCompa->hauteur+1;
            } else { // un élément déjà existant à gauche, nouvelle comparaison
                _ajouterNoeud(nodeCompa->gauche,nodeAjout);
            }
        } else { // à droite
            if(nodeCompa->droit == nullptr){ // aucun élément à droite, ajout du nouvel élément
                nodeCompa->droit = nodeAjout;
                nodeAjout->hauteur = nodeCompa->hauteur+1;
            } else { // un élément déjà existant à droite, nouvelle comparaison
                _ajouterNoeud(nodeCompa->droit,nodeAjout);
            }
        }
    } else { // comparaison sur Y
        if(nodeAjout->p->y < nodeCompa->p->y){ // à gauche
            if(nodeCompa->gauche == nullptr){ // aucun élément à gauche, ajout du nouvel élément
                nodeCompa->gauche = nodeAjout;
                nodeAjout->hauteur = nodeCompa->hauteur+1;
            } else { // un élément déjà existant à gauche, nouvelle comparaison
                _ajouterNoeud(nodeCompa->gauche,nodeAjout);
            }
        } else { // à droite
            if(nodeCompa->droit == nullptr){ // aucun élément à droite, ajout du nouvel élément
                nodeCompa->droit = nodeAjout;
                nodeAjout->hauteur = nodeCompa->hauteur+1;
            } else { // un élément déjà existant à droite, nouvelle comparaison
                _ajouterNoeud(nodeCompa->droit,nodeAjout);
            }
        }
    }

}

void KdTree::supprimerNoeud(KdNode* node){
    
}

KdNode* KdTree::chercherNoeud(Point* point){
    KdNode* node = racine;
    while(true){
        if(node->p->x == point->x && node->p->y == point->y){ // si point trouvé
                return node;
            }
        if(node->hauteur%2 == 0){ // test sur X
            if(point->x < node->p->x){ // si plus petit sur X
                if (node->gauche == nullptr){ // si pas d'élément à gauche
                    return nullptr;
                } else { // sinon on continue la recherche
                    node = node->gauche;
                }
            } else { // si plus grand ou égal
                if (node->droit == nullptr){ // si pas d'élément à droite
                    return nullptr;
                } else { // sinon on continue la recherche
                    node = node->droit;
                }
            }
        } else { // test sur Y
            if(point->y < node->p->y){ // si plus petit sur Y
                if (node->gauche == nullptr){ // pas d'élément à gauche
                    return nullptr;
                } else { // sinon on continue
                    node = node->gauche;
                }
            } else { // si plus grand ou égal
                if (node->droit == nullptr){ // si pas d'élément à droite
                    return nullptr;
                } else { // sinon on continue
                    node = node->droit;
                }
            }
        }
    }
    return nullptr;
}
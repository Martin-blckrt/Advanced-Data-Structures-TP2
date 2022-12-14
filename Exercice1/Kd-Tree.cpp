#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <math.h>

using namespace std;

class Point {
public:
    char* nom;
	int x;
    int y;

	Point(char* nomP,int pointX,int pointY) : nom(nomP), x(pointX), y(pointY) {}

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

    inline void ajouterNoeud(Point* point);
    inline KdNode* supprimerNoeud(KdNode*);
    inline Point* chercherNoeud(Point* point);
    inline KdNode* plusProcheVoisin(Point*);
    inline void afficherArbre();
    inline KdNode* minNode(KdNode*,KdNode*,KdNode*,int);
    inline KdNode* getNode(Point*);

    private:
        
    KdNode* racine;
    int nbelem;
    inline KdNode* _supprimerNoeud(KdNode* ,KdNode*);
    inline void _detruire(KdNode*);
    inline void _ajouterNoeud(KdNode*,Point*);
    inline KdNode* _bestNodeToReplace(KdNode*,int);
    inline void _plusProcheVoisin(KdNode*,Point*,KdNode*,double*);
    inline double _calculDistance(Point*,Point*);
    inline void _afficherArbre(KdNode*);
    inline KdNode* _getNode(KdNode*,Point*);

};

KdTree::KdTree(){
    racine = nullptr;
    nbelem = 0;
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

void KdTree::ajouterNoeud(Point* PointS){
    _ajouterNoeud(racine,PointS);
}

void KdTree::_ajouterNoeud(KdNode* nodeCompa,Point* PointS){
    if(nodeCompa == nullptr){ // cas racine
    KdNode* NouvNode = new KdNode(PointS);
    racine = NouvNode;
    racine->hauteur = 0;
    nbelem++;
    }else if(nodeCompa->p->x == PointS->x && nodeCompa->p->y == PointS->y){// identique
        // fin
        printf("point deja existant\n");
    }else {
        if(nodeCompa->hauteur % 2 == 0){ // comparaison sur X
            if(PointS->x < nodeCompa->p->x){ // à gauche
                if(nodeCompa->gauche == nullptr){ // aucun élément à gauche, ajout du nouvel élément
                    KdNode* NouvNode = new KdNode(PointS);
                    nodeCompa->gauche = NouvNode;
                    NouvNode->hauteur = nodeCompa->hauteur+1;
                    nbelem++;
                } else { // un élément déjà existant à gauche, nouvelle comparaison
                    _ajouterNoeud(nodeCompa->gauche,PointS);
                }
            } else { // à droite
                if(nodeCompa->droit == nullptr){ // aucun élément à droite, ajout du nouvel élément
                    KdNode* NouvNode = new KdNode(PointS);
                    nodeCompa->droit = NouvNode;
                    NouvNode->hauteur = nodeCompa->hauteur+1;
                    nbelem++;
                } else { // un élément déjà existant à droite, nouvelle comparaison
                    _ajouterNoeud(nodeCompa->droit,PointS);
                }
            }
        } else { // comparaison sur Y
            if(PointS->y < nodeCompa->p->y){ // à gauche
                if(nodeCompa->gauche == nullptr){ // aucun élément à gauche, ajout du nouvel élément
                    KdNode* NouvNode = new KdNode(PointS);
                    nodeCompa->gauche = NouvNode;
                    NouvNode->hauteur = nodeCompa->hauteur+1;
                    nbelem++;
                } else { // un élément déjà existant à gauche, nouvelle comparaison
                    _ajouterNoeud(nodeCompa->gauche,PointS);
                }
            } else { // à droite
                if(nodeCompa->droit == nullptr){ // aucun élément à droite, ajout du nouvel élément
                    KdNode* NouvNode = new KdNode(PointS);
                    nodeCompa->droit = NouvNode;
                    NouvNode->hauteur = nodeCompa->hauteur+1;
                    nbelem++;
                } else { // un élément déjà existant à droite, nouvelle comparaison
                    _ajouterNoeud(nodeCompa->droit,PointS);
                }
            }
        }
    }
}

KdNode* KdTree::supprimerNoeud(KdNode* nodeS){
    return _supprimerNoeud(racine, nodeS);
}


KdNode* KdTree::_supprimerNoeud(KdNode* ActualNode,KdNode* nodeS){
    // Point actuel existant
    if (ActualNode == nullptr){
        return nullptr;
    }    
    // trouver la coordonnées que l'on compare
    int cd = ActualNode->hauteur % 2;
    
    if (ActualNode->p->x == nodeS->p->x && ActualNode->p->y == nodeS->p->y){
        // si l'enfant est non null
        if (ActualNode->droit != nullptr){
            
            // trouver le noeud minimum dans le sous-arbre droit
            KdNode *meilleureNode = _bestNodeToReplace(ActualNode->droit, cd);
  
            // on copie le minimum
            ActualNode->p = meilleureNode->p;
  
            // On supprime le noeud récursivement
            ActualNode->droit = _supprimerNoeud(ActualNode->droit, meilleureNode);
        } else if (ActualNode->gauche != nullptr) {
            KdNode *meilleureNode = _bestNodeToReplace(ActualNode->gauche, cd);
            ActualNode->p = meilleureNode->p;
            ActualNode->droit = _supprimerNoeud(ActualNode->gauche, meilleureNode);
        } else {// Si le noeud à supprimer est une feuille 
            nbelem--;
            delete ActualNode;
            return nullptr;
        }
        return ActualNode;
    } else {
        
    }
    
    // 2) si le noeud actuel ne contient pas le point que l'on veut supprimer
    if(cd==0){
        
        if (nodeS->p->x < ActualNode->p->x){
            ActualNode->gauche = _supprimerNoeud(ActualNode->gauche, nodeS);
        }else {
            ActualNode->droit = _supprimerNoeud(ActualNode->droit, nodeS);
        }
    } else {
        if (nodeS->p->y < ActualNode->p->y){
            ActualNode->gauche = _supprimerNoeud(ActualNode->gauche, nodeS);
        }else {
            ActualNode->droit = _supprimerNoeud(ActualNode->droit, nodeS);
        }
    }
    return ActualNode;
}

KdNode* KdTree::_bestNodeToReplace(KdNode* node,int dim){ // dim x ou y
    if(node == nullptr){
        return nullptr;
    } else {
        if(node->hauteur%2 == dim){
            if(node->gauche == nullptr){
                return node;
            } else {
                return _bestNodeToReplace(node->gauche,dim);
            }
        } else {
            return minNode(node,
                    _bestNodeToReplace(node->gauche,dim),
                    _bestNodeToReplace(node->droit,dim),
                    dim);
        }
    }
    
}

KdNode* KdTree::minNode(KdNode *main, KdNode *left, KdNode *right, int d){
    KdNode *res = main;
    if(d == 0){ // sur X
        if (left != NULL && left->p->x < res->p->x){
            res = left;
        }
        if (right != NULL && right->p->x < res->p->x){
            res = right;
        }
    } else { // sur Y
        if (left != NULL && left->p->y < res->p->y){
            res = left;
        }
        if (right != NULL && right->p->y < res->p->y){
            res = right;
        }
    }
    return res;
}

Point* KdTree::chercherNoeud(Point* point){
    KdNode* node = racine;
    while(true){
        if(node->p->x == point->x && node->p->y == point->y){ // si point trouvé
                printf("Point %s aux coordonnees (%d,%d) \n",node->p->nom,node->p->x,node->p->y);
                return node->p;
            }
        if(node->hauteur%2 == 0){ // test sur X
            if(point->x < node->p->x){ // si plus petit sur X
                if (node->gauche == nullptr){ // si pas d'élément à gauche
                    printf("Point non trouve\n");
                    return nullptr;
                } else { // sinon on continue la recherche
                    node = node->gauche;
                }
            } else { // si plus grand ou égal
                if (node->droit == nullptr){ // si pas d'élément à droite
                    printf("Point non trouve\n");
                    return nullptr;
                } else { // sinon on continue la recherche
                    node = node->droit;
                }
            }
        } else { // test sur Y
            if(point->y < node->p->y){ // si plus petit sur Y
                if (node->gauche == nullptr){ // pas d'élément à gauche
                    printf("Point non trouve\n");
                    return nullptr;
                } else { // sinon on continue
                    node = node->gauche;
                }
            } else { // si plus grand ou égal
                if (node->droit == nullptr){ // si pas d'élément à droite
                    printf("Point non trouve\n");
                    return nullptr;
                } else { // sinon on continue
                    node = node->droit;
                }
            }
        }
    }
    return nullptr;
}

KdNode* KdTree::plusProcheVoisin(Point* point){

    KdNode* meilleur = nullptr;   
	double _minDist = std::numeric_limits<double>::max();

    _plusProcheVoisin(racine,point,meilleur,&_minDist);
    printf("Distance minimale : %2f \n",_minDist);
    
    return meilleur;
}

void KdTree::_plusProcheVoisin(KdNode* node,Point* point,KdNode* meilleur,double* mindist){
    
    if (node == nullptr){
        return ;
    }

    const double dist = _calculDistance(point, node->p);
    if (dist < *mindist){
        *mindist = dist;
        meilleur = node;
        printf("Point %s aux coordonnees (%d,%d) \n",node->p->nom,node->p->x,node->p->y);
    }
    
    if(node->hauteur%2 == 0){
        if(point->x < node->p->x){
            _plusProcheVoisin( node->gauche,point, meilleur, mindist);
            const double diff = fabs(point->x - node->p->x);
            if (diff < *mindist){
                _plusProcheVoisin(node->droit, point, meilleur, mindist);
            }
        } else {
            _plusProcheVoisin( node->droit,point, meilleur, mindist);
            const double diff = fabs(point->x - node->p->x);
            if (diff < *mindist){
                _plusProcheVoisin(node->gauche, point, meilleur, mindist);
            }
        }
        
        
    } else {
        if(point->y < node->p->y){
            _plusProcheVoisin( node->gauche,point, meilleur, mindist);
            const double diff = fabs(point->y - node->p->y);
            if (diff < *mindist){
                _plusProcheVoisin(node->droit, point, meilleur, mindist);
            }
        } else {
            _plusProcheVoisin( node->droit,point, meilleur, mindist);
            const double diff = fabs(point->y - node->p->y);
            if (diff < *mindist){
                _plusProcheVoisin(node->gauche, point, meilleur, mindist);
            }
        }
        
    }
        
}

double KdTree::_calculDistance(Point* p1,Point* p2){

    return sqrt(pow(p2->x-p1->x,2)+pow(p2->y-p1->y,2));

}

void KdTree::afficherArbre(){
    _afficherArbre(racine);
}

void KdTree::_afficherArbre(KdNode* node){
    if (node != nullptr){
        printf("Point %s aux coordonnees (%d,%d) \n",node->p->nom,node->p->x,node->p->y);
        if(node->gauche != nullptr){
            _afficherArbre(node->gauche);
        }
        if (node->droit != nullptr){
            _afficherArbre(node->droit);
        }
    }  
}

KdNode* KdTree::getNode(Point* point){
    return _getNode(racine,point);
}

KdNode* KdTree::_getNode(KdNode* node,Point* point){
    if(node == nullptr){ // cas racine
        return nullptr;
    }else if(node->p->x == point->x && node->p->y == point->y){// identique
        // fin
        return node;
    }else {
        if(node->hauteur%2 == 0){
            if(point->x < node->p->x){
                return _getNode(node->gauche, point);
            } else {
                return _getNode(node->droit, point);
            }
        } else {
            if(point->y < node->p->y){
                return _getNode(node->gauche, point);
            } else {
                return _getNode(node->droit, point);
            }
        }
    }
}

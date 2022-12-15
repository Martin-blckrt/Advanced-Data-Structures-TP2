#include <iostream>
#include <string>
#include "Kd-Tree.cpp"

using namespace std;

int main()
{
    // la compilation entraine des warning mais le code marche 
    KdTree* tree = new KdTree();
    // Création des points 
    Point* point1 = new Point("A",52,17);
    Point* point2 = new Point("B",23,49);
    Point* point3 = new Point("C",70,20);
    Point* point4 = new Point("D",68,42);
    Point* point5 = new Point("E",7,17);
    Point* point6 = new Point("F",21,22);
    Point* point7 = new Point("G",98,45);
    Point* point8 = new Point("H",67,71);
    Point* point9 = new Point("I",12,58);
    Point* point10 = new Point("J",28,34);
    Point* point12 = new Point("L",72,18);

    // test ajout points
    tree->ajouterNoeud(point1);
    tree->ajouterNoeud(point2);

    tree->afficherArbre();
    printf("------------------------\n");

    tree->ajouterNoeud(point3);
    tree->ajouterNoeud(point4);
    tree->ajouterNoeud(point5);
    tree->ajouterNoeud(point6);
    tree->ajouterNoeud(point7);
    tree->ajouterNoeud(point8);
    tree->ajouterNoeud(point9);
    tree->ajouterNoeud(point10);
    tree->ajouterNoeud(point12);

    tree->afficherArbre();
    printf("------------------------\n");
    Point* point11 = new Point("K",12,58);
    tree->ajouterNoeud(point11);
    printf("------------------------\n");
    tree->chercherNoeud(point1);
    tree->chercherNoeud(point6);
    tree->chercherNoeud(point10);

    printf("------------------------\n");

    tree->supprimerNoeud(tree->getNode(point2));
    tree->afficherArbre();
    printf("------------------------\n");
    Point* point13 = new Point("M",40,70);
    KdNode* node = tree->plusProcheVoisin(point13);
    //printf("Point %d",node->p->x);
    
    //printf("Point %s aux coordonnees (%d,%d) \n",node->p->nom,node->p->x,node->p->y);

    //cout << "Marche\n";
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.

#include <iostream>
#include <utility>

#include "Graph.h"

using namespace std;

pair<string, GroupStrategy> getInput();

int main()
{
	cout << "DEVOIR 2 - Exercice 2" << endl;

	std::cout << "\n>> Choix utilisateur" << std::endl;

    // Choose 'eu' or 'in' and group strategy 
    pair<string, GroupStrategy> choices = getInput();

    auto* g = new Graph(choices.first, choices.second);

	return 0;
}

pair<string, GroupStrategy> getInput() {

	int res;
	pair<string, GroupStrategy> ans;
	
	cout << endl << "Choisir une zone" << endl;
	cout << "1: Europe   2: Inde" << endl;
	
	while (!(cin >> res) || (res != 1 && res != 2)) {

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalide car different de 1 ou 2" << endl << "Recommencez: ";
	}
	cout << endl;

	switch (res)
	{
	case 1:
		ans.first = "eu";
		break;
	case 2:
		ans.first = "in";
		break;
	default:
		break;
	}

	cout << "Choisir une strategie de regroupement" << endl;
	cout << "1: Page   2: Domaine   3: Hote" << endl;

	while (!(cin >> res) || (res != 1 && res != 2 && res != 3)) {

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalide car different de 1, 2 ou 3" << endl << "Recommencez: ";
	}
	cout << endl;

	switch (res)
	{
	case 1:
		ans.second = GroupStrategy::page;
		break;
	case 2:
		ans.second = GroupStrategy::domain;
		break;
	case 3:
		ans.second = GroupStrategy::host;
		break;
	default:
		break;
	}

	return ans;
}

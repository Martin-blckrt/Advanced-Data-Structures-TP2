#include <iostream>

#include "Graph.h"

using namespace std;

string getInput();

int main()
{
	cout << "DEVOIR 2 - Exercice 2" << endl;

    // Choose 'eu' or 'in' 
    string ans = getInput();

    auto* g = new Graph(ans);

	return 0;
}

string getInput() {

	int res;
	string ans;
	
	cout << endl << "Choisir une zone" << endl;
	cout << "1: Europe - 2: India" << endl << endl;
	
	while (!(cin >> res) || (res != 1 && res != 2)) {

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalide car different de 1 ou 2" << endl << "Recommencez: ";
	}
	cout << endl;

	// make switch statement if we add more zones
	if (res == 1)
	{
		ans = "eu";
	} else {
		ans = "in";
	}

	return ans;
}

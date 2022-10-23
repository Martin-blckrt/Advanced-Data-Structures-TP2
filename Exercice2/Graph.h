#pragma once

#include <iostream>
#include <string>

class Graph
{
public:
	Graph(std::string);

private:

};

Graph::Graph(std::string zone) {
	if (zone == "in") {

		std::cout << "Zone: India" << std::endl;

	}
	else {
		std::cout << "Zone: Europe" << std::endl;
	}

}


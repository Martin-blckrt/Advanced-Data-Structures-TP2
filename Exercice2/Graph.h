#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "GraphElem.h"

class Graph
{
public:
	Graph(std::string);

	friend void readFileContent(Graph& gr, std::ifstream&, bool);

	std::vector<Node> getNodes() { return nodes; };
	std::vector<Edge> getEdges() { return edges; };


private:
	std::vector<Node> nodes;
	std::vector<Edge> edges;
};

Graph::Graph(std::string zone) {

	std::string file_str;	// file name str

	std::cout << "Graph initialisation\n" << std::endl;

	if (zone == "in") {

		std::cout << "Zone: India" << std::endl;
		file_str = "in-2004";
	}
	else {
		std::cout << "Zone: Europe" << std::endl;
		file_str = "eu-2005";
		
	}

	std::string node_str = "resources/" + file_str + ".nodes.txt";
	std::string edge_str = "resources/" + file_str + ".edges.txt";


	std::ifstream nodefile(node_str);
	std::ifstream edgefile(edge_str);

	std::cout << "Reading " << node_str << std::endl;
	readFileContent(*this, nodefile, true);

	std::cout << "Reading " << edge_str << std::endl;
	readFileContent(*this, edgefile, false);	

	std::cout << "Graph completed" << std::endl;
}

void readFileContent(Graph& gr, std::ifstream& file, bool isNode) {

	std::string line;	// file content str
	
	if (file.is_open()) {

		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			int nb1, nb2; std::string line_url;	// pattern variables

			if (isNode) {
				ss >> nb1 >> nb2 >> line_url;  // pattern declaration
				if (!ss) { continue; }	// if pattern doesnt match, ignore line
				
				gr.nodes.push_back(Node(nb1, nb2, line_url));
			}
			else {
				ss >> nb1 >> nb2;
				if (!ss) { continue; }	// if pattern doesnt match, ignore line

				gr.edges.push_back(Edge(nb1, nb2));
			}
		}
			
		std::cout << "Done" << std::endl;
		file.close();
	}
}


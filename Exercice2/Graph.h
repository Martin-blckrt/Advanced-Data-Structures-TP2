#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <utility>

#include "GraphElem.h"

class Graph
{
public:

	GroupStrategy strat;

	Graph(std::string, GroupStrategy);
	~Graph();

	std::map<int, std::vector<int>> readEdgeFileContent(std::ifstream&);
	void readNodeFileContent(std::ifstream&, std::map<int, std::vector<int>>&);

	std::vector<Node*>& getNodes() { return nodes; };
	std::vector<Edge*>& getEdges() { return edges; };
	std::vector<HyperSet*>& getHyperSets() { return hypersets; };
	std::vector<HyperEdge*>& getHyperEdges() { return hyperedges; };

	void createHyperSets();
	void createHyperEdges();

private:
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;

	std::vector<HyperSet*> hypersets;
	std::vector<HyperEdge*> hyperedges;
};

Graph::Graph(std::string zone, GroupStrategy strategy = GroupStrategy::page) {

	std::string file_str;	// file name str

	std::cout << ">> Graph initialisation\n" << std::endl;

	if (zone == "in") {

		std::cout << "Zone: India" << std::endl;
		file_str = "in-2004";
	}
	else {
		std::cout << "Zone: Europe" << std::endl;
		file_str = "eu-2005";
		
	}

	std::string strat_str = "";
	switch (strategy)
	{
	case GroupStrategy::page:
		strat_str = "By page";
		break;
	case GroupStrategy::domain:
		strat_str = "By domain";
		break;
	case GroupStrategy::host:
		strat_str = "By host";
		break;
	default:
		break;
	}

	std::cout << "GroupStrategy: " + strat_str << std::endl << std::endl;
	strat = strategy;

	std::cout << ">> Graph construction\n" << std::endl;
	std::string node_str = "resources/" + file_str + ".nodes.txt";
	std::string edge_str = "resources/" + file_str + ".edges.txt";

	std::ifstream nodefile(node_str);
	std::ifstream edgefile(edge_str);

	std::cout << "Reading " << edge_str << std::endl;
	std::map<int, std::vector<int>> adj_map = readEdgeFileContent(edgefile);

	std::cout << "Reading " << node_str << std::endl << std::endl;
	readNodeFileContent(nodefile, adj_map);

	std::cout << ">> Graph links\n" << std::endl;
	std::cout << "Creating hyper sets... ";
	createHyperSets();

	std::cout << "Creating hyper edges... ";
	createHyperEdges();
	
	std::cout << "Graph completed\n" << std::endl;
}

Graph::~Graph() {

	for (auto n : nodes)
		delete n;

	for (auto e : edges)
		delete e;

	for (auto hs : hypersets)
		delete hs;

	for (auto he : hyperedges)
		delete he;
}

std::map<int, std::vector<int>> Graph::readEdgeFileContent(std::ifstream& file) {

	std::string line;	// line content
	std::map<int, std::vector<int>> adj_map;

	if (file.is_open()) {

		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			int nb1, nb2;	// pattern variables

			ss >> nb1 >> nb2;	// pattern declaration

			// if it matches, register data
			if (ss)
			{
				// no self referencing link
				if (nb1 != nb2)
				{
					adj_map[nb1].push_back(nb2);
					adj_map[nb1].shrink_to_fit();

					edges.push_back(new Edge(nb1, nb2));
				}	
			}
		}

		file.close();
	}

	return adj_map;
}

void Graph::readNodeFileContent(std::ifstream& file, std::map<int, std::vector<int>>& adj_map) {

	std::string line;

	if (file.is_open()) {

		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			int nb1, nb2; std::string line_url;

			ss >> nb1 >> nb2 >> line_url;  

			if (ss)
				nodes.push_back(new Node(nb1, nb2, line_url, adj_map[nb1]));
		}

		file.close();
	}
}

void Graph::createHyperSets() {

	std::string content;
	std::map<std::string, std::vector<Node*>> by_url_list;
	std::regex urlRe("^.*://([^/?:]+)/?.*$");

	if (strat == GroupStrategy::page)
	{
		hypersets.reserve(nodes.size());

		for (auto page : nodes) {

			HyperSet* h = new HyperSet({ page });
			hypersets.push_back(h);
		}

	} else if(strat == GroupStrategy::domain) {

		for (auto page : nodes) {

			std::string url = page->getUrl();

			content = std::regex_replace(url, urlRe, "$1");

			size_t pos = content.find('.');
			content.erase(0, pos + 1);

			by_url_list[content].push_back(page);
		}

		hypersets.reserve(by_url_list.size());

		for (auto url_list : by_url_list)
		{
			HyperSet* h = new HyperSet(url_list.second);
			hypersets.push_back(h);
		}
	}
	else if (strat == GroupStrategy::host) {

		for (auto page : nodes) {

			std::string url = page->getUrl();

			content = std::regex_replace(url, urlRe, "$1");

			by_url_list[content].push_back(page);
		}

		hypersets.reserve(by_url_list.size());

		for (auto url_list : by_url_list)
		{
			HyperSet* h = new HyperSet(url_list.second);
			hypersets.push_back(h);
		}
	}

	hypersets.shrink_to_fit();

	std::cout << "Done" << std::endl;
}

void Graph::createHyperEdges() {

	//TODO: Check si des edges sont présentes (attribut strength)

	if (!hypersets.empty())
	{
		for (auto hyperset : hypersets)
		{
			for (auto node : hyperset->getSet())
			{
				for (auto dest_ids : node->getAdj())
				{
					HyperEdge* hedge = new HyperEdge(hyperset, dest_ids, 0);
					hyperedges.push_back(hedge);
				}
			}
		}
	}

	std::cout << "Done" << std::endl;
}

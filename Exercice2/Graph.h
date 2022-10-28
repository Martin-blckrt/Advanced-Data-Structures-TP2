#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <utility>
#include <algorithm>

#include "GraphElem.h"

class Graph
{
public:

	GroupStrategy strat;

	Graph(const std::string&, GroupStrategy);
	~Graph();

	std::map<int, std::vector<int>> readEdgeFileContent(std::ifstream&);
	void readNodeFileContent(std::ifstream&, std::map<int, std::vector<int>>&);

	std::vector<Node*>& getNodes() { return nodes; };
	std::vector<Edge*>& getEdges() { return edges; };
	std::vector<HyperSet*>& getHyperSets() { return hypersets; };
	std::vector<HyperEdge*>& getHyperEdges() { return hyperedges; };

	void createHyperSets();
	void createHyperEdges();

	std::map<HyperSet*, std::vector<HyperEdge*>> findDuplicates();
	void mesureWeights();
	void removeDuplicates();

private:
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;

	std::vector<HyperSet*> hypersets;
	std::vector<HyperEdge*> hyperedges;
};

Graph::Graph(const std::string& zone, GroupStrategy strategy = GroupStrategy::page) {

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

	std::string strat_str;
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

	std::cout << "Calculating weights... ";
	mesureWeights();

	std::cout << "Removing duplicate edges... ";
	removeDuplicates();
	
	std::cout << "hello " << hyperedges[11]->getWeight() << std::endl;

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
	std::vector<Node*> node_list(1);
	std::map<std::string, std::vector<Node*>> by_url_list;
	std::regex urlRe("^.*://([^/?:]+)/?.*$");

	if (strat == GroupStrategy::page)
	{
		hypersets.reserve(nodes.size());

		for (auto page : nodes) {

			node_list = { page };
			auto* h = new HyperSet(node_list);
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
			auto* h = new HyperSet(url_list.second);
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
			auto* h = new HyperSet(url_list.second);
			hypersets.push_back(h);
		}
	}

	hypersets.shrink_to_fit();

	std::cout << "Done" << std::endl;
}

void Graph::createHyperEdges() {

	std::map<int, std::vector<HyperSet*>> target_map;

	for (auto hyperset : hypersets)
		for (auto node : hyperset->getSet())
			for (auto dest_id : node->getAdj())
				target_map[dest_id].push_back(hyperset);


	hyperedges.reserve(target_map.size());

	for (const auto& targ : target_map)
	{
		for (auto hs : targ.second)
		{
			auto* hedge = new HyperEdge(hs, targ.first);
			hyperedges.push_back(hedge);
		}
	}

	std::cout << "Done" << std::endl;
}

std::map<HyperSet*, std::vector<HyperEdge*>> Graph::findDuplicates()
{
	std::map<HyperSet*, std::vector<HyperEdge*>> hit_list;

	for (int i = 0; i < hyperedges.size(); i++)
	{
		auto curr_src = hyperedges[i]->getSource();

		if (hit_list.count(curr_src) != 0)
		{
			if (hit_list[curr_src][0]->getDestination() == hyperedges[i]->getDestination())
				hit_list[curr_src].push_back(hyperedges[i]);

		}
		else {
			hit_list[curr_src].push_back(hyperedges[i]);
		}
	}

	return hit_list;
}

void Graph::mesureWeights() {

	auto hit_list = findDuplicates();

	for (auto hit : hit_list)
	{
		auto ed_list = hit.second;
		size_t bonus = ed_list.size();

		for(auto ed : ed_list)
			ed->setWeight(bonus);
	}

	std::cout << "Done" << std::endl;
}

void Graph::removeDuplicates() {

	auto hit_list = findDuplicates();
	
	std::vector<HyperEdge*> result;

	for (auto hit : hit_list)
	{
		auto elem = hit.second.back();
		size_t vec_size = hit.second.size();

		result.push_back(elem);

		for (int i = 0; i < vec_size - 1; i++)
			delete hit.second[i];
	}
	
	hyperedges = result;
	hyperedges.shrink_to_fit();

	std::cout << "Done" << std::endl;
}

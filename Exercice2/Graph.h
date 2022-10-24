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

	friend void readFileContent(Graph& gr, std::ifstream&, bool);

	std::vector<Node> getNodes() { return nodes; };
	std::vector<Edge> getEdges() { return edges; };
	std::vector<HyperSet> getHyperSets() { return hypersets; };
	std::vector<HyperEdge> getHyperEdges() { return hyperedges; };

	void createHyperSets();
	void createHyperEdges();

private:
	std::vector<Node> nodes;
	std::vector<Edge> edges;

	std::vector<HyperSet> hypersets;
	std::vector<HyperEdge> hyperedges;
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

	std::cout << "Reading " << node_str << std::endl;
	readFileContent(*this, nodefile, true);

	std::cout << "Reading " << edge_str << std::endl;
	readFileContent(*this, edgefile, false);

	std::cout << "Creating hyper sets" << std::endl;
	createHyperSets();

	std::cout << "Creating hyper edges" << std::endl;
	createHyperEdges();
	
	std::cout << "Graph completed\n" << std::endl;
}

void readFileContent(Graph& gr, std::ifstream& file, bool isNode) {

	std::string line;	// line content

	if (file.is_open()) {

		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			int nb1, nb2; std::string line_url;	// pattern variables

			if (isNode) {

				ss >> nb1 >> nb2 >> line_url;  // pattern declaration

				// if it matches, register data
				if (ss)
					gr.nodes.push_back(Node(nb1, nb2, line_url)); 	
			}
			else {

				ss >> nb1 >> nb2;

				if (ss)
					gr.edges.push_back(Edge(nb1, nb2));
			}
		}
			
		std::cout << "Done" << std::endl;
		file.close();
	}
}

void Graph::createHyperSets() {

	std::string content;
	std::map<std::string, std::vector<Node>> by_url_list;
	std::regex urlRe("^.*://([^/?:]+)/?.*$");

	if (strat == GroupStrategy::page)
	{
		hypersets.reserve(nodes.size());

		for (auto& page : nodes) {

			HyperSet h({ page });
			hypersets.push_back(h);
		}

	} else if(strat == GroupStrategy::domain) {

		for (auto& page : nodes) {

			std::string url = page.getUrl();

			content = std::regex_replace(url, urlRe, "$1");

			size_t pos = content.find('.');
			content.erase(0, pos + 1);

			by_url_list[content].push_back(page);
		}

		hypersets.reserve(by_url_list.size());

		for (auto& url_list : by_url_list)
		{
			HyperSet h(url_list.second);
			hypersets.push_back(h);
		}
	}
	else if (strat == GroupStrategy::host) {

		for (auto& page : nodes) {

			std::string url = page.getUrl();

			content = std::regex_replace(url, urlRe, "$1");

			by_url_list[content].push_back(page);
		}

		hypersets.reserve(by_url_list.size());

		for (auto& url_list : by_url_list)
		{
			HyperSet h(url_list.second);
			hypersets.push_back(h);
		}
	}

	std::cout << "Done" << std::endl;
}

void Graph::createHyperEdges() {

	// < pair<id source, noeud>, pair<set concerné, nb d'occurence> >

	std::map<Node, HyperSet> targets;
	
	if (!hypersets.empty())
	{
		for (auto& hyperset : hypersets) {
			
			auto current_set = hyperset.getSet();

			for (auto& node : current_set)
			{
				for (auto& edge : edges) {

					if (edge.getSource() == node.getId()){
						
						targets[node] = hyperset;
					}
				}
			}

			hyperedges.reserve(targets.size());

			for (auto& targ : targets)
			{
				//TODO: fix strength
				HyperEdge he(targ.second, targ.first, 0);
				hyperedges.push_back(he);
			}

			targets.clear();
		}
	}

	std::cout << "Done" << std::endl;
}

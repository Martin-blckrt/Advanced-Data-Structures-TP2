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
#include <deque>

#include "GraphElem.h"

using namespace std;

class Graph
{
public:

	int greatestId;
	GroupStrategy strat;

	Graph(const string&, GroupStrategy);
	~Graph();

	map<int, vector<int>> readEdgeFileContent(ifstream&);
	void readNodeFileContent(ifstream&, map<int, vector<int>>&);

	vector<Node*>& getNodes() { return nodes; };
	vector<Edge*>& getEdges() { return edges; };
	vector<HyperSet*>& getHyperSets() { return hypersets; };
	vector<HyperEdge*>& getHyperEdges() { return hyperedges; };

	void createHyperSets();
	void createHyperEdges();

	void balanceGraph();

	void applyAlgorithm();

	void Indegree();
	void PageRank();

private:
	vector<Node*> nodes;
	vector<Edge*> edges;

	vector<HyperSet*> hypersets;
	vector<HyperEdge*> hyperedges;
};

Graph::Graph(const string& zone, GroupStrategy strategy = GroupStrategy::page) {

	string file_str;	// file name str

	cout << ">> Graph initialisation\n" << endl;

	if (zone == "in") {

		cout << "Zone: India" << endl;
		file_str = "in-2004";
	}
	else {
		cout << "Zone: Europe" << endl;
		file_str = "eu-2005";
		
	}

	string strat_str;
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

	cout << "GroupStrategy: " + strat_str << endl << endl;
	strat = strategy;

	cout << ">> Graph construction\n" << endl;
	string node_str = "resources/" + file_str + ".nodes.txt";
	string edge_str = "resources/" + file_str + ".edges.txt";

	ifstream nodefile(node_str);
	ifstream edgefile(edge_str);

	cout << "Reading " << edge_str << endl;
	map<int, vector<int>> adj_map = readEdgeFileContent(edgefile);

	cout << "Reading " << node_str << endl << endl;
	readNodeFileContent(nodefile, adj_map);

	cout << ">> Graph links\n" << endl;
	cout << "Creating hyper sets... ";
	createHyperSets();

	cout << "Creating hyper edges... ";
	createHyperEdges();

	cout << "Calculating weights and removing duplicate edges... ";
	balanceGraph();

	cout << "Graph completed\n" << endl;

	cout << ">> Algorithms\n" << endl;
	applyAlgorithm();

	cout << "End of the program" << endl;
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

map<int, vector<int>> Graph::readEdgeFileContent(ifstream& file) {

	string line;	// line content
	map<int, vector<int>> adj_map;

	if (file.is_open()) {

		while (getline(file, line))
		{
			stringstream ss(line);
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

void Graph::readNodeFileContent(ifstream& file, map<int, vector<int>>& adj_map) {

	string line;

	if (file.is_open()) {

		while (getline(file, line))
		{
			stringstream ss(line);
			int nb1, nb2; string line_url;

			ss >> nb1 >> nb2 >> line_url;  

			if (ss) 
			{
				if (nb1 > greatestId)
					greatestId = nb1;

				nodes.push_back(new Node(nb1, nb2, line_url, adj_map[nb1]));
			}
		}

		file.close();
	}
}

void Graph::createHyperSets() {

	string content;
	vector<Node*> node_list(1);
	map<string, vector<Node*>> by_url_list;
	regex urlRe("^.*://([^/?:]+)/?.*$");

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

			string url = page->getUrl();

			content = regex_replace(url, urlRe, "$1");

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

			string url = page->getUrl();

			content = regex_replace(url, urlRe, "$1");

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

	cout << "Done" << endl;
}

void Graph::createHyperEdges() {

	map<int, vector<HyperSet*>> target_map;

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

	cout << "Done" << endl;
}

void Graph::balanceGraph()
{
	int dupliCount = 0;
	map<HyperSet*, vector<HyperEdge*>> hit_list;
	map<HyperSet*, vector<HyperEdge*>> orphans;

	for (int i = 0; i < hyperedges.size(); i++)
	{
		auto curr_src = hyperedges[i]->getSource();

		if (hit_list.count(curr_src) != 0)
		{
			if (hit_list[curr_src][0]->getDestination() == hyperedges[i]->getDestination())
				hit_list[curr_src].push_back(hyperedges[i]);
			else
				orphans[curr_src].push_back(hyperedges[i]);
		}
		else {
			hit_list[curr_src].push_back(hyperedges[i]);
		}
	}

	vector<HyperEdge*> result;

	for (auto hit : hit_list)
	{
		auto elem = hit.second.back();
		size_t vec_size = hit.second.size();

		elem->setWeight(vec_size);
		result.push_back(elem);

		for (int i = 0; i < vec_size - 1; i++)
		{
			delete hit.second[i];
			dupliCount++;
		}
	}

	for (auto orph : orphans)
	{
		for (auto elem : orph.second)
		{
			elem->setWeight(1);
			result.push_back(elem);
		}
	}

	hyperedges = result;
	hyperedges.shrink_to_fit();

	cout << "Done, removed " << dupliCount << " duplicates" << endl;
}

void Graph::applyAlgorithm()
{
	int res;

	cout << "Choisir un algorithme" << endl;
	cout << "1: Indegree   2: PageRank" << endl;

	while (!(cin >> res) || (res != 1 && res != 2)) {

		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalide car different de 1 ou 2" << endl << "Recommencez: ";
	}
	cout << endl;

	if (res == 1)
		Indegree();
	else
		PageRank();
}

void Graph::Indegree()
{
	size_t node_size = nodes.size();
	
	map<int, int> in_degree;
	vector<int> top_order;

	for (auto edge : hyperedges) {
		in_degree[edge->getDestination()]++;
		if (edge->getWeight() == 0)
			cout << "yeya" << endl;
	}
		
	
	// needs to act as queue, so add to back and remove from front
	deque<int> q;
	
	for (int i = 0; i < node_size; i++)
	{
		int curr_id = nodes[i]->getId();

		if (in_degree.count(curr_id) == 0)
			in_degree[curr_id] = 0;
		
		q.push_back(curr_id);
	}
		
	// Count of visited sets
	int cnt = 0;

	while (!q.empty())
	{
		int u = q.front();
		q.pop_front();
		top_order.push_back(u);

		// Do stuff

		cnt++;
	}

	if (cnt != node_size) {
		cout << "There is a cycle in the graph\n";
		return;
	}

	// Print topological order
	for (auto top : top_order)
		cout << top << " ";
	cout << endl;
}

void Graph::PageRank()
{

}
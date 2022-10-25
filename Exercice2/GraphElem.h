#pragma once

#include <iostream>
#include <string>
#include <algorithm>

// Utility header

// Granularité
enum class GroupStrategy { page, domain, host };

class Node
{
public:

	Node(int id, int dg, std::string lk, std::vector<int> adj) :node_id(id), out_degree(dg), url(lk), adjNodes(adj) {};

	int getId() { return node_id; };
	int getDegree() { return out_degree; };
	std::string getUrl() { return url; };

	std::vector<int> getAdj() { return adjNodes; };

	bool operator==(const Node& other) const {
		
		return node_id == other.node_id && out_degree == other.out_degree && url == other.url;
	};


private:
	int node_id;
	int out_degree;

	std::string url;

	std::vector<int> adjNodes;
};

class Edge 
{
public: 

	Edge(int src, int dst): source(src), dest(dst){};

	int getSource() { return source; };
	int getDestination() { return dest; };

private:
	int source;
	int dest;
};

class HyperSet
{
public:

	HyperSet(){};
	HyperSet(std::vector<Node*> node_set) : set(node_set){};

	bool setEmpty() { return set.empty(); };

	bool contains(int target_id) {

		bool res = false;

		for (auto& node : set) {
			if (node->getId() == target_id)
			{
				res = true;
			}
		}
		return res;
	};

	Node* getNodeById(int target_id) {

		Node * res = nullptr;

		for (auto& node : set) {
			if (node->getId() == target_id)
			{
				res = node;
			}
		}
		return res;
	}

	void addToSet(Node* node) {

		if (!(std::find(set.begin(), set.end(), node) != set.end()))
			set.push_back(node);
	};

	std::vector<Node*> getSet() { return set; };
	
private:
	std::vector<Node*> set;
};

class HyperEdge
{
public:

	HyperEdge(HyperSet src, int dst, int stgth = 0) :
		srcSet(src), destNode(dst), strength(stgth) {};

	HyperSet& getSource() { return srcSet; };
	int getDestination() { return destNode; };

private:
	HyperSet srcSet;
	int destNode;

	int strength;
};
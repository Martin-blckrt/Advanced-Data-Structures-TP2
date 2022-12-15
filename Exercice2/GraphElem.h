#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

// Utility header

// Granularite
enum class GroupStrategy { page, domain, host };

class Node
{
public:

	Node(int id, int dg, std::string lk, std::vector<int>& adj) :node_id(id), out_degree(dg), url(std::move(lk)), adjNodes(adj) {};

	int getId() const { return node_id; };
	int getDegree() const { return out_degree; };
	std::string getUrl() { return url; };

	std::vector<int>& getAdj() { return adjNodes; };

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

	int getSource() const { return source; };
	int getDestination() const { return dest; };

private:
	int source;
	int dest;
};

class Bloc
{
public:

	explicit Bloc(int bid, std::vector<Node*>& node_set) : bloc_id(bid), set(node_set){};

	bool blocEmpty() { return set.empty(); };

	bool contains(int target_id) {

		bool res = false;

		for (auto& node : set) {
			if (node->getId() == target_id)
			{
				res = true;
				break;
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
				break;
			}
		}
		return res;
	}

	void addToSet(Node* node) {

		if (!(std::find(set.begin(), set.end(), node) != set.end()))
			set.push_back(node);
	};

	std::vector<Node*> getSet() { return set; };
	int getBlocId() { return bloc_id; };
	
private:
	int bloc_id;
	std::vector<Node*> set;
};

class HyperEdge
{
public:

	HyperEdge(Bloc* src, int dst, size_t w = 0) :
		srcSet(src), destNode(dst), weight(w) {};

	Bloc* getSource() { return srcSet; };
	int getDestination() const { return destNode; };
	size_t getWeight() const { return weight; };

	void increaseWeight() { weight++; };
	void setWeight(size_t bonus) { weight = bonus; };

	bool operator==(const HyperEdge& other) const {

		bool attr = srcSet == other.srcSet && destNode == other.destNode;
		return attr && (this != &other);
	};

private:
	Bloc* srcSet;
	int destNode;

	size_t weight;
};
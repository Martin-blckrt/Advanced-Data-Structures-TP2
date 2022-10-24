#pragma once

#include <iostream>
#include <string>

// Granularité
enum GroupStrategy { page, domain, host };

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

class Node
{
public:

	Node(int id, int dg, std::string lk):node_id(id), out_degree(dg), url(lk) {};

	int getId() { return node_id; };
	int getDegree() { return out_degree; };
	std::string getUrl() { return url; };

private:
	int node_id;
	int out_degree;

	std::string url;

};


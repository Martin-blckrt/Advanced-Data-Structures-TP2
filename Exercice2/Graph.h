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
#include <numeric>
#include <limits>
#include <iomanip>

#include "GraphElem.h"

using namespace std;

class Graph {
public:

    int greatestId;
    GroupStrategy strat;
    map<int, Bloc *> idmap;

    Graph(const string &, GroupStrategy);

    ~Graph();

    map<int, vector<int>> readEdgeFileContent(ifstream &);

    void readNodeFileContent(ifstream &, map<int, vector<int>> &);

    vector<Node *> &getNodes() { return nodes; };

    vector<Edge *> &getEdges() { return edges; };

    vector<Bloc *> &getBlocs() { return blocs; };

    vector<HyperEdge *> &getHyperEdges() { return hyperedges; };

    void createBlocs();

    void createHyperEdges();

    void balanceGraph();

    void applyAlgorithm();

    void Indegree();

    void PageRank();

private:
    vector<Node *> nodes;
    vector<Edge *> edges;

    vector<Bloc *> blocs;
    vector<HyperEdge *> hyperedges;
};

Graph::Graph(const string &zone, GroupStrategy strategy = GroupStrategy::page) {

    string file_str;    // file name str

    cout << ">> Initialisation du graphe\n" << endl;

    if (zone == "in") {

        cout << "Zone: India" << endl;
        file_str = "in-2004";
    } else {
        cout << "Zone: Europe" << endl;
        file_str = "eu-2005";

    }

    string strat_str;
    switch (strategy) {
        case GroupStrategy::page:
            strat_str = "par page";
            break;
        case GroupStrategy::domain:
            strat_str = "par domaine";
            break;
        case GroupStrategy::host:
            strat_str = "par hote";
            break;
        default:
            break;
    }

    cout << "Strategie de regroupement: " + strat_str << endl << endl;
    strat = strategy;

    cout << ">> Construction du graphe\n" << endl;
    string node_str = "resources/" + file_str + ".nodes.txt";
    string edge_str = "resources/" + file_str + ".edges.txt";

    ifstream nodefile(node_str);
    ifstream edgefile(edge_str);

    cout << "Lecture du fichier " << edge_str << endl;
    map<int, vector<int>> adj_map = readEdgeFileContent(edgefile);

    cout << "Lecture du fichier " << node_str << endl << endl;
    readNodeFileContent(nodefile, adj_map);

    cout << ">> Connexions et regroupements\n" << endl;
    cout << "Creation des blocs... ";
    createBlocs();

    cout << "Creations des aretes... ";
    createHyperEdges();

	cout << "Suppression des doubles et calcul des poids" << endl;
	balanceGraph();

    cout << "Construction terminee\n" << endl;

    cout << ">> Algorithmes\n" << endl;
    applyAlgorithm();

    cout << endl << "Fin du programme" << endl;
}

Graph::~Graph() {

    for (auto &n: nodes)
        delete n;

    for (auto &e: edges)
        delete e;

    for (auto &b: blocs)
        delete b;

    for (auto &h: hyperedges)
        delete h;
}

map<int, vector<int>> Graph::readEdgeFileContent(ifstream &file) {

    string line;    // line content
    map<int, vector<int>> adj_map;

    if (file.is_open()) {

        while (getline(file, line)) {
            stringstream ss(line);
            int nb1, nb2;    // pattern variables

            ss >> nb1 >> nb2;    // pattern declaration

            // if it matches, register data
            if (ss) {
                // no self referencing link
                if (nb1 != nb2) {
                    adj_map[nb1].push_back(nb2);
                    edges.push_back(new Edge(nb1, nb2));
                }
            }
        }

        file.close();
    }

    return adj_map;
}

void Graph::readNodeFileContent(ifstream &file, map<int, vector<int>> &adj_map) {

    string line;

    // utilise les memes concepts que readEdgeFileContent
    if (file.is_open()) {

        while (getline(file, line)) {
            stringstream ss(line);
            int nb1, nb2;
            string line_url;

            ss >> nb1 >> nb2 >> line_url;

            if (ss) {
                if (nb1 > greatestId)
                    greatestId = nb1;

                nodes.push_back(new Node(nb1, nb2, line_url, adj_map[nb1]));
            }
        }

        file.close();
    }
}

void Graph::createBlocs() {

    string content;
    vector<Node *> node_list(1);
    map<string, vector<Node *>> by_url_list;
    regex urlRe("^.*://([^/?:]+)/?.*$");
    int id = 0;

    // creation des blocs selon la strategie choisie
    if (strat == GroupStrategy::page) {
        blocs.reserve(nodes.size());

        for (auto page: nodes) {

            node_list = {page};
            auto *h = new Bloc(id, node_list);
            blocs.push_back(h);

            idmap[page->getId()] = h;
            id++;
        }

    } else if (strat == GroupStrategy::domain) {

        for (auto page: nodes) {

            string url = page->getUrl();

            content = regex_replace(url, urlRe, "$1");

            size_t pos = content.find('.');
            content.erase(0, pos + 1);

            by_url_list[content].push_back(page);
        }

        blocs.reserve(by_url_list.size());

        for (auto url_list: by_url_list) {
            auto *h = new Bloc(id, url_list.second);
            blocs.push_back(h);

            id++;
            for (auto &nodeid: url_list.second)
                idmap[nodeid->getId()] = h;
        }
    } else if (strat == GroupStrategy::host) {

        for (auto page: nodes) {

            string url = page->getUrl();

            content = regex_replace(url, urlRe, "$1");

            by_url_list[content].push_back(page);
        }

        blocs.reserve(by_url_list.size());

        for (auto url_list: by_url_list) {
            auto *h = new Bloc(id, url_list.second);
            blocs.push_back(h);
            id++;

            for (auto &nodeid: url_list.second)
                idmap[nodeid->getId()] = h;
        }
    }

    blocs.shrink_to_fit();

    cout << "Termine" << endl;
}

void Graph::createHyperEdges() {

    map<int, vector<Bloc *>> target_map;

    for (auto bloc: blocs)
        for (auto node: bloc->getSet())
            for (auto dest_id: node->getAdj())
                target_map[dest_id].push_back(bloc);


    hyperedges.reserve(target_map.size());

    for (const auto &targ: target_map) {
        for (auto hs: targ.second) {
            if (idmap.count(targ.first) > 0)
            {
                auto* hedge = new HyperEdge(hs, targ.first);
                hyperedges.push_back(hedge);
            }
        }
    }

    cout << "Termine" << endl;
}

void Graph::balanceGraph() {
    int dupliCount = 0;
    map<Bloc *, vector<HyperEdge *>> hit_list;
    map<Bloc *, vector<HyperEdge *>> orphans;

    // Compte les hyperedges pointant au même endroit à partir de la même source
    for (int i = 0; i < hyperedges.size(); i++) {
        auto curr_src = hyperedges[i]->getSource();

        if (hit_list.count(curr_src) != 0) {
            if (hit_list[curr_src][0]->getDestination() == hyperedges[i]->getDestination())
                hit_list[curr_src].push_back(hyperedges[i]);
            else
                orphans[curr_src].push_back(hyperedges[i]);
        } else {
            hit_list[curr_src].push_back(hyperedges[i]);
        }
    }

    vector<HyperEdge *> result;

    // mets à jour les poids et supprime les doublons
    for (auto hit: hit_list) {
        auto elem = hit.second.back();
        size_t vec_size = hit.second.size();

        elem->setWeight(vec_size);
        result.push_back(elem);

        for (int i = 0; i < vec_size - 1; i++) {
            delete hit.second[i];
            dupliCount++;
        }
    }

    for (auto orph: orphans) {
        for (auto elem: orph.second) {
            elem->setWeight(1);
            result.push_back(elem);
        }
    }

    hyperedges = result;
    hyperedges.shrink_to_fit();

    cout << "Termine, " << dupliCount << " doublons supprimes" << endl;
}

void Graph::applyAlgorithm() {
    int res;

    cout << "Choisir un algorithme" << endl;
    cout << "PageRank crée un fichier 'PageRank.txt', assurez vous que le fichier nexiste pas avant de lancer" << endl;

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
	map<int, int> in_degree;
	map<Bloc*, vector<HyperEdge*>> edge_to_set;

	vector<int> top_order;

	for (auto& bloc : blocs)
		in_degree[bloc->getBlocId()] = 0;

	for (auto& edge : hyperedges)
	{
		int dest = edge->getDestination();

		if (idmap.count(dest) > 0)
			edge_to_set[idmap[dest]].push_back(edge);

		if (in_degree.count(dest) > 0)
			in_degree[idmap[dest]->getBlocId()]++;
	}
		
	// needs to act as queue, so add to back and remove from front
	deque<int> q;
	
	for (auto& inset : in_degree)
		if (inset.second == 0)
			q.push_back(inset.first);


	// Count of visited sets
	int cnt = 0;

    while (!q.empty()) {
        auto u = q.front();
        q.pop_front();

        top_order.push_back(u);

		auto related_edges = edge_to_set[idmap[u]];

		for (auto& red : related_edges) {

			auto ds = red->getDestination();

			if (--in_degree[idmap[ds]->getBlocId()] == 0)
				q.push_back(idmap[ds]->getBlocId());
		}

		// Do stuff

        cnt++;
    }

    // si presence de cycle, on arrete le programme, Indegree n'est pas faisable
	if (cnt != blocs.size()) {
		cout << "Cycle detecte, fin de l algorithme\n";
		cout << cnt << " visites sur " << blocs.size() << endl;
		return;
	}

	int indent = 0;

	// Print topological order
	for (auto top : top_order) {
		cout << top << " ";
		
		indent++;
		
		if (indent % 15 == 0)
			cout << endl;

	}
	cout << endl;
}

void Graph::PageRank() {

    // number of iterations
    int n = 3;

    map<int, double> rank;

    // jump factor
    double d = 0.15;

    // number of pages
    double N = nodes.size();

    // initialize the rank of each page
    for (auto v: nodes)
        rank[v->getId()] = 1 / (double)N;

    // map a bloc and the number of outlinks it has
    map<Bloc *, int> outlinks;

    // initialize map
    for (auto bloc: blocs)
        outlinks[bloc] = 0;

    // update it
    for (auto edge: hyperedges)
        if (outlinks.count(edge->getSource()) > 0)
            outlinks[edge->getSource()] += 1;

    // make multi-map that links blocs to pages
    multimap<int, Bloc *> hyperMap;
    map<int, vector<Bloc*>> hitmap;

    for (auto edge : hyperedges) {
        
        auto dest = edge->getDestination();
        auto src = edge->getSource();

        if (hitmap.count(dest) == 0) {

            hitmap[dest] = vector<Bloc*>({ src });
            hyperMap.insert({ dest, src });
        }
        else {
            bool found = false;

            for (auto hit : hitmap[dest])
                if (hit == src)
                    found = true;

            if (!found)
            {
                hitmap[dest].push_back(src);
                hyperMap.insert({ dest, src });
            }
        }
        
    }

    // map a bloc and it's rank
    map<Bloc *, double> blocRank;

    while (n > 0) {

        // update the bloc ranks at each iteration
        for (auto b: blocs) {
            double r = 0.0;
            for (auto p: b->getSet())
                r += rank[p->getId()];
            r = (double)r / b->getSet().size();
            blocRank[b] = r;
        }

        // sum of no outlink/nb_blocs for a page v
        double sum_noOutlink_nbBlocs = 0.0;

        // get all blocs without outlinks and update the no outlink/nb_blocs sum
        for (auto b: outlinks)
            if (b.second == 0)
                sum_noOutlink_nbBlocs += (double)blocRank[b.first] / blocs.size();

        // for all pages
        for (auto v: nodes) {

            // sum of rank/outlinks for a page v
            double sum_rank_outlink = 0.0;

            // We now need to get each bloc pointing to page v
            // with the hypermap (initialized at the beginning), get all Blocs* linked to the key V.getId() (= the page)

            typedef multimap<int, Bloc*>::iterator MMAPIterator;
            pair<MMAPIterator, MMAPIterator> result = hyperMap.equal_range(v->getId());

            // iterate over the range
            for (MMAPIterator it = result.first; it != result.second; it++)
                sum_rank_outlink += (double)blocRank[it->second] / outlinks[it->second];

            // update the rank of page v
            rank[v->getId()] = (double)(d / N) + ((double)((1 - d) * (sum_rank_outlink + sum_noOutlink_nbBlocs)));
        }
        n--;
    }

    vector<pair<int, double>> pairs;
    for (auto itr = rank.begin(); itr != rank.end(); ++itr)
        pairs.push_back(*itr);

    std::sort(pairs.begin(), pairs.end(), [=](std::pair<int, double> &a, std::pair<int, double> &b) {
             return a.second < b.second;
         }
    );
    
    fstream resultfile;
    resultfile.open("PageRank.txt", ios_base::out);

    for (int i = 0; i < pairs.size(); i++)
        resultfile << "Page " << pairs[i].first << " has a score of " << std::setprecision(10) << std::fixed << pairs[i].second << endl;
 
    resultfile.close();

    cout << "Results added in PageRank.txt file" << endl;
}
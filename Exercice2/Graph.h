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

    cout << ">> Graph initialisation\n" << endl;

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
    createBlocs();

    cout << "Creating hyper edges... ";
    createHyperEdges();

    cout << "Calculating weights and removing duplicate edges... ";
    //balanceGraph();

    cout << "Graph completed\n" << endl;

    cout << ">> Algorithms\n" << endl;
    applyAlgorithm();

    cout << "End of the program" << endl;
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

    if (strat == GroupStrategy::page) {
        blocs.reserve(nodes.size());

        for (auto page: nodes) {

            node_list = {page};
            auto *h = new Bloc(node_list);
            blocs.push_back(h);

            idmap[page->getId()] = h;
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
            auto *h = new Bloc(url_list.second);
            blocs.push_back(h);

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
            auto *h = new Bloc(url_list.second);
            blocs.push_back(h);

            for (auto &nodeid: url_list.second)
                idmap[nodeid->getId()] = h;
        }
    }

    blocs.shrink_to_fit();

    cout << "Done" << endl;
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
            auto *hedge = new HyperEdge(hs, targ.first);
            hyperedges.push_back(hedge);
        }
    }

    cout << "Done" << endl;
}

void Graph::balanceGraph() {
    int dupliCount = 0;
    map<Bloc *, vector<HyperEdge *>> hit_list;
    map<Bloc *, vector<HyperEdge *>> orphans;

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

    cout << "Done, removed " << dupliCount << " duplicates" << endl;
}

void Graph::applyAlgorithm() {
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


void Graph::Indegree() {
    size_t bloc_size = blocs.size();

    map<Bloc *, int> in_degree;
    map<Bloc *, vector<HyperEdge *>> edge_to_set;

    vector<Bloc *> top_order;

    for (auto &edge: hyperedges) {
        auto src = edge->getSource();
        int dest = edge->getDestination();

        if (in_degree.count(src) == 0)
            in_degree[src] = 0;

        edge_to_set[idmap[dest]].push_back(edge);
        in_degree[idmap[dest]]++;
    }

    // needs to act as queue, so add to back and remove from front
    deque<Bloc *> q;

    for (auto &inset: in_degree)
        if (inset.second == 0)
            q.push_back(inset.first);

    // Count of visited sets
    int cnt = 0;

    while (!q.empty()) {
        auto u = q.front();
        q.pop_front();

        top_order.push_back(u);

        auto related_edges = edge_to_set[u];

        for (auto &red: related_edges) {

            auto src = red->getSource();

            if (--in_degree[src] == 0)
                q.push_back(src);
        }


        // Do stuff

        cnt++;
    }

    if (cnt != bloc_size) {
        cout << "There is a cycle in the graph\n";
        cout << "visited " << cnt << " out of " << bloc_size << endl;
        return;
    }

    // Print topological order
    for (auto top: top_order)
        cout << top << " ";
    cout << endl;
}

void Graph::PageRank() {

    // used this : https://swang21.medium.com/what-is-googles-pagerank-algorithm-d0ac17cbc167
    // tweaked it to use blocs and hyperedges

    // number of iterations
    int n = 3;

    map<int, double> rank;

    // jump factor
    double d = 0.15;

    // number of pages
    double N = nodes.size();

    // initialize the rank of each page
    for (auto v: nodes)
        rank[v->getId()] = 1 / N;

    // map a bloc and the number of outlinks it has
    map<Bloc *, int> outlinks;
    // initialize map
    for (auto bloc: blocs)
        outlinks[bloc] = 0;
    // update it
    for (auto edge: hyperedges)
        outlinks[edge->getSource()] += 1;

    // map a bloc and it's rank
    map<Bloc *, double> blocRank;

    while (n > 0) {

        // update the bloc ranks at each iteration
        for (auto b: blocs) {
            double r = 0;
            for (auto p: b->getSet())
                r += rank[p->getId()];
            r = r / b->getSet().size();
            blocRank[b] = r;
        }

        // sum of no outlink/nb_blocs for a page v
        double sum_noOutlink_nbBlocs = 0;
        // get all blocs without outlinks and update the no outlink/nb_blocs sum
        for (auto b: outlinks)
            if (b.second == 0)
                sum_noOutlink_nbBlocs += blocRank[b.first] / blocs.size();

        // for all pages
        for (auto v: nodes) {

            // sum of rank/outlinks for a page v
            double sum_rank_outlink = 0;

            // we need to get each bloc pointing to page v so we iterate in hyperedges
            for (auto edge: hyperedges) {

                // if a bloc points to page v, update the rank/outlink sum
                if (edge->getDestination() == v->getId())
                    sum_rank_outlink += blocRank[edge->getSource()] / outlinks[edge->getSource()];
            }

            // update the rank of page v
            rank[v->getId()] = d / N + (1 - d) * (sum_rank_outlink + sum_noOutlink_nbBlocs);
        }
        n--;
    }
    vector<pair<int, int>> pairs;
    for (auto itr = rank.begin(); itr != rank.end(); ++itr)
        pairs.push_back(*itr);

    sort(pairs.begin(), pairs.end(), [=](std::pair<int, int>& a, std::pair<int, int>& b)
         {
             return a.second < b.second;
         }
    );

    for (auto page : pairs)
        cout << "Page " << page.first << " has a score of " << page.second << endl;
}
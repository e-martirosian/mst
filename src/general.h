#pragma once

#include <bits/stdc++.h>
#include <random>
#include <functional>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#define EPS 0.00001
#define INF 1000000000

using namespace std;

using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS,
        no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::pair<int, int> E;


struct EDGE {
    int a;
    int b;
    int weight;

    int value() const {
        return weight;
    }

};

typedef vector<vector<int>> GRAPH_MATRIX;

struct GRAPH {
    int number_vertices;
    int number_edges;
    vector<vector<EDGE>> edge_list;
    vector<EDGE> edges;
    vector<int> vertices;
    vector<vector<bool>> adj_matrix;
};

double getDiff(chrono::_V2::system_clock::time_point time) {
    return (double) chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - time).count();
}

static bool find_cycle(int x, int prev, vector<vector<int>> &edge_list, vector<int> &was) {
    was[x] = 1;
    for (auto &p: edge_list[x]) {
        if (prev == p)
            continue;
        if (was[p] == 1) {
            return true;
        } else if (was[p] == 2) {
            throw;
        } else {
            if (find_cycle(p, x, edge_list, was))
                return true;
        }
    }
    was[x] = 2;
    return false;
}

bool is_connected(vector<int> vertices, vector<vector<int>> edge_list) {
    int number_vertices = vertices.size();
    vector<int> was(number_vertices, 0);

    for (int i = 0; i < number_vertices; i++) {
        if (was[vertices[i]] == 0 && find_cycle(vertices[i], -1, edge_list, was))
            return false;
    }

    return true;
}

struct MST {
    int sum = 0;
    vector<EDGE> edges;

    void add(EDGE edge) {
        edges.push_back(edge);
        sum += edge.weight;
    }

    bool is_mst(GRAPH &graph) {
        if (edges.size() != graph.number_vertices - 1)
            return false;

        vector<vector<int>> edge_list(graph.number_vertices);

        for (auto &edge: edges) {
            edge_list[edge.a].push_back(edge.b);
            edge_list[edge.b].push_back(edge.a);
            if (!graph.adj_matrix[edge.a][edge.b]) {
                return false;
            }
        }

        set<pair<int, int>> distinct_edges;
        for (auto &edge: edges)
            distinct_edges.insert({edge.a, edge.b});

        if (distinct_edges.size() != graph.number_vertices - 1)
            throw;

        return is_connected(graph.vertices, edge_list);
    }
};

vector<bool> was;

int dfs(int x, const GRAPH &graph) {
    int ans = 1;
    was[x] = true;
    for (auto i : graph.edge_list[x]) {
        int j = (i.a == x ? i.b : i.a);
        if (!was[j])
            ans += dfs(j, graph);
    }
    return ans;
}

pair<Graph, GRAPH> generateGraph(int num_nodes, int num_edges) {
    srand(time(nullptr));

    while (true) {
        vector<vector<EDGE>> edge_list;
        vector<EDGE> edges;
        edge_list.resize(num_nodes);
        vector<vector<bool>> adj_matrix(num_nodes, vector<bool>(num_nodes, false));

        std::vector<int> weights;
        std::uniform_int_distribution<int> random_int;
        vector<int> vertices;
        for (int i = 0; i < num_nodes; i++)
            vertices.push_back(i);
        set<int> node_added;
        for (int i = 0; i < num_nodes; i++) {
            for (int j = i + 1; j < num_nodes; j++) {
                if (rand() % 100 <= (100 * num_edges) / (((num_nodes - 1) * num_nodes) / 2)) {
                    node_added.insert(i);
                    node_added.insert(j);
                    adj_matrix[i][j] = true;
                    adj_matrix[j][i] = true;
                    weights.push_back(rand() % 1000 + 1);
                    edges.push_back({i, j, weights.back()});
                    edge_list[i].push_back({i, j, weights.back()});
                    edge_list[j].push_back({i, j, weights.back()});
                }
                if (edges.size() + num_nodes - node_added.size() == num_edges)
                    break;
            }
            if (edges.size() + num_nodes - node_added.size() == num_edges)
                break;
        }

        for (int i = 0; i < num_nodes; i++) {
            if (node_added.find(i) == node_added.end() || num_nodes - i + edges.size() == num_edges) {
                int v1 = i;
                int v2 = vector<int>(node_added.begin(), node_added.end())[rand() % node_added.size()];
                node_added.insert(v1);
                adj_matrix[v2][v1] = true;
                adj_matrix[v1][v2] = true;
                weights.push_back(rand() % 1000 + 1);
                edges.push_back({min(v1, v2), max(v1, v2), weights.back()});
                edge_list[v1].push_back({min(v1, v2), max(v1, v2), weights.back()});
                edge_list[v2].push_back({min(v1, v2), max(v1, v2), weights.back()});
            }
        }

        Graph g_type2(num_nodes);
        boost::property_map<Graph, boost::edge_weight_t>::type weightmap = get(boost::edge_weight, g_type2);
        for (std::size_t j = 0; j < num_edges; ++j) {
            Edge e;
            bool inserted;
            tie(e, inserted) = add_edge(edges[j].a, edges[j].b, g_type2);
            if (!inserted)
                throw;
            weightmap[e] = weights[j];
        }

        GRAPH g_type1{num_nodes, num_edges, edge_list, edges, vertices, adj_matrix};

        was = vector<bool>(num_nodes, false);
        if (dfs(0, g_type1) != num_nodes)
            continue;
        return {g_type2, g_type1};
    }
}

MST true_mst(Graph &g) {
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, g);
    std::vector<Edge> spanning_tree;

    kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));

    MST mst = MST();

    graph_traits<Graph>::edge_iterator eiter, eiter_end;
    for (tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
        if (std::find(spanning_tree.begin(), spanning_tree.end(), *eiter)
            != spanning_tree.end()) {
            mst.add(EDGE{static_cast<int>(source(*eiter, g)), static_cast<int>(target(*eiter, g)), weight[*eiter]});
        }
    }
    return mst;
}

void write_graph(const GRAPH &graph, const string &filename = "outputs/graph.txt") {
    ofstream out("../../" + filename);
    out << graph.number_vertices << " " << graph.number_edges << "\n";
    for (auto edge: graph.edges)
        out << edge.a << " " << edge.b << " " << edge.weight << "\n";
    out.close();
}

void write_answer(const MST &mst, const string &filename) {
    ofstream out("../../" + filename);
    for (auto edge: mst.edges)
        out << edge.a << " " << edge.b << " " << edge.weight << "\n";
    out.close();
}

struct TIMES {
    double kruskal_time;
    double prim_time;
    double true_time;
};
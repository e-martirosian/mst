#include "./src/general.h"
#include "./src/algorithms/Prim.cpp"
#include "./src/algorithms/Kruskal.cpp"


pair<Graph, GRAPH> read_graph() {
//    ifstream in("../../output/graph.txt");
    int num_nodes, num_edges;
    cin >> num_nodes >> num_edges;

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
    for (int _ = 0; _ < num_edges; _++) {
        int i, j, w;
        cin >> i >> j >> w;
        node_added.insert(i);
        node_added.insert(j);
        adj_matrix[i][j] = true;
        adj_matrix[j][i] = true;
        weights.push_back(w);
        edges.push_back({i, j, weights.back()});
        edge_list[i].push_back({i, j, weights.back()});
        edge_list[j].push_back({i, j, weights.back()});
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

    write_graph(g_type1, "outputs/graph.txt");

    return {g_type2, g_type1};
}

int main() {
    auto graph = read_graph();

    auto kruskal_time_begin = chrono::system_clock::now();
    MST mst = kruskal(graph.second);
    auto kruskal_time = getDiff(kruskal_time_begin);

    auto true_time_begin = chrono::system_clock::now();
    MST mst2 = true_mst(graph.first);
    auto true_time = getDiff(true_time_begin);

    auto prim_time_begin = chrono::system_clock::now();
    MST mst3 = prim(graph.second);
    auto prim_time = getDiff(prim_time_begin);

    write_answer(mst, "outputs/kruskal.txt");
    write_answer(mst2, "outputs/true.txt");
    write_answer(mst3, "outputs/prim.txt");

    if (mst.is_mst(graph.second) && mst3.is_mst(graph.second) && mst.sum == mst2.sum && mst3.sum == mst2.sum) {
        cout << kruskal_time << " " << prim_time << " " << true_time << "\n";
        return 0;
    }

    throw;
}

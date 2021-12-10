#pragma once
#include "../data structure/DSU.cpp"

MST kruskal(GRAPH &graph) {
    DSU dsu = DSU(graph.number_vertices);

    sort(graph.edges.begin(), graph.edges.end(), [](EDGE a, EDGE b) {
        return a.weight < b.weight;
    });

    for (auto & i : graph.vertices){
        dsu.make_set(i);
    }

    MST mst = MST();

    for (auto &edge: graph.edges) {
        if (dsu.find_set(edge.a) != dsu.find_set(edge.b)) {
            mst.add(edge);
        }
        dsu.union_sets(edge.a, edge.b);
    }

    return mst;
}
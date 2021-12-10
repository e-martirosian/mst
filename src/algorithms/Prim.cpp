#pragma once

#include "../data structure/DHeap.cpp"

MST prim(GRAPH &graph) {
    DHeap heap(graph.number_edges);

    MST mst = MST();

    for (auto &edge: graph.edge_list[graph.vertices[0]]) {
        heap.add(edge);
    }

    vector<bool> added(graph.number_vertices, false);
    added[graph.vertices[0]] = true;

    while (mst.edges.size() < graph.number_vertices - 1) {
        int new_node = -1;
        EDGE min_edge;
        do {
            min_edge = heap.find_min();
            new_node = (added[min_edge.a] ? min_edge.b : min_edge.a);
            heap.remove_min();
        } while (added[new_node]);

        mst.add(min_edge);
        added[new_node] = true;

        for (auto &edge: graph.edge_list[new_node])
            if (!added[edge.a] || !added[edge.b]) {
                heap.add(edge);
            }
    }

    return mst;
}
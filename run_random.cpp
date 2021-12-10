#include "./src/general.h"
#include "./src/algorithms/Prim.cpp"
#include "./src/algorithms/Kruskal.cpp"

TIMES run_test(int n, int m) {
    auto graph = generateGraph(n, m);

    write_graph(graph.second, "outputs/graph.txt");

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

    if (mst.is_mst(graph.second) && mst3.is_mst(graph.second) && mst.sum == mst2.sum && mst3.sum == mst2.sum)
        return {kruskal_time, prim_time, true_time};

    throw;
}


int main() {
    srand(time(nullptr));
    int n = rand() % 10 + 1;
    run_test(n, rand() % (((n - 1) * (n - 2)) / 2) + n - 1);
    cout << "Passed!";
}

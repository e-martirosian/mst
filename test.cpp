#include "./src/general.h"
#include "./src/algorithms/Kruskal.cpp"
#include "./src/algorithms/Prim.cpp"

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
    ofstream out("../../outputs/results_4.txt");
    out << "n,m,kruskal_time,prim_time,true_time\n";

    for (int n: {10, 100, 200, 500, 1000, 2000, 3000}) {
        int step = (((n + 1) * n) / 2 - (n - 1)) / 7;
        for (int m = max(n - 1, step); m <= ((n + 1) * n) / 2; m += step) {
            cout << n << " " << m << "\n";
            double kruskal_time = 0;
            double prim_time = 0;
            double true_time = 0;
            int ATTEMPTS = 20;
            for (int _ = 0; _ < ATTEMPTS; _++) {
                TIMES times = run_test(n, m);
                kruskal_time += times.kruskal_time;
                prim_time += times.prim_time;
                true_time += times.true_time;
            }
            out << n << "," << m << "," << kruskal_time/ATTEMPTS << "," << prim_time/ATTEMPTS << "," << true_time/ATTEMPTS << "\n";
        }
    }

    out.close();
}

#pragma once

#include "../general.h"

class DHeap {

public:
    vector<EDGE> items;

    explicit DHeap(int max_size, int d = 2) {
        this->d = d;
        items.resize(max_size);
        items_size = 0;
    }

    int min_child(int v) {
        int min_ch = -1;
        for (int child = v * d + 1; child <= min(items_size - 1, v * d + d); ++child)
            if (min_ch == -1 || items[min_ch].value() > items[child].value())
                min_ch = child;
        return min_ch;
    }

    void shift_down(int v) {
        int min_ch = min_child(v);
        while (v != -1 && min_ch != -1 && items[v].value() > items[min_ch].value()) {
            swap(items[v], items[min_ch]);
            v = min_ch;
            min_ch = min_child(v);
        }
    }

    int parent(int v) {
        if (v == 0)
            return -1;
        return (v - 1) / d;
    }

    void shift_up(int v) {
        while (v != 0 && parent(v) != -1 && items[v].value() < items[parent(v)].value()) {
            swap(items[v], items[parent(v)]);
            v = parent(v);
        }
    }

    void add(EDGE item) {
        items[items_size] = item;
        items_size++;
        shift_up(items_size - 1);
    }

    EDGE find_min() {
        return items[0];
    }

    void remove_min() {
        swap(items[0], items[items_size - 1]);
        items_size--;
        shift_down(0);
    }

    int items_size;
    int d;
};

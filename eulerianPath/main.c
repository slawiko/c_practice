#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Edge {
    int from;
    int to;
    bool is_visited;
};

int cmp(struct Edge* a, struct Edge* b) {
    return a->to - b->to;
}

int main() {
    size_t n;
    scanf("%zu", &n);
    struct Edge* edges = (struct Edge*)malloc((n - 1) * sizeof(struct Edge));
    int* result = (int*)malloc((2 * n - 1) * sizeof(int));

    for (size_t i = 0; i < n - 1; i++) {
        scanf("%d", &(edges[i].from));
        scanf("%d", &(edges[i].to));
        edges[i].is_visited = false;
    }

    qsort(edges, n - 1, sizeof(struct Edge), cmp);

    for (size_t i = 0; i < n - 1; i++) {

    }
    return 0;
}
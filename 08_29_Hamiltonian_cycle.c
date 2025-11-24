#include <stdio.h>
#include <stdbool.h>

#define MAX 20

int graph[MAX][MAX];
int path[MAX];
int n;

bool isSafe(int v, int pos) {
    if (graph[path[pos - 1]][v] == 0)
        return false;

    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return false;

    return true;
}

bool hamCycleUtil(int pos) {
    if (pos == n) {
        if (graph[path[pos - 1]][path[0]] == 1)
            return true;
        else
            return false;
    }

    for (int v = 1; v < n; v++) {
        if (isSafe(v, pos)) {
            path[pos] = v;

            if (hamCycleUtil(pos + 1))
                return true;

            path[pos] = -1; 
        }
    }

    return false;
}
bool hasHamiltonianCycle() {
    for (int i = 0; i < n; i++)
        path[i] = -1;

    path[0] = 0; 

    return hamCycleUtil(1);
}

int main() {
    int e, u, v;

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter number of edges: ");
    scanf("%d", &e);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = 0;

    printf("Enter %d edges (u v):\n", e);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &v);
        graph[u][v] = 1;
        graph[v][u] = 1; 
    }

    if (hasHamiltonianCycle())
        printf("Hamiltonian Cycle exists.\n");
    else
        printf("Hamiltonian Cycle does not exist.\n");

    return 0;
}

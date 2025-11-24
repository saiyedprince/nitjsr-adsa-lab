#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 100

int graph[MAX][MAX];
int color[MAX];
int n;

bool isBipartite(int start) {
    int queue[MAX], front = 0, rear = 0;
    queue[rear++] = start;
    color[start] = 1; 

    while (front < rear) {
        int u = queue[front++];

        for (int v = 0; v < n; v++) {
            if (graph[u][v]) {
                if (color[v] == 0) {
                    color[v] = -color[u]; 
                    queue[rear++] = v;
                } else if (color[v] == color[u]) {
                    return false; 
                }
            }
        }
    }

    return true;
}

int main() {
    int e, u, v;

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter number of edges: ");
    scanf("%d", &e);
    for (int i = 0; i < n; i++) {
        color[i] = 0;
        for (int j = 0; j < n; j++)
            graph[i][j] = 0;
    }

    printf("Enter %d edges (u v):\n", e);
    for (int i = 0; i < e; i++) {
        scanf("%d %d", &u, &v);
        graph[u][v] = 1;
        graph[v][u] = 1; 
    }

    bool bipartite = true;
    for (int i = 0; i < n; i++) {
        if (color[i] == 0) {
            if (!isBipartite(i)) {
                bipartite = false;
                break;
            }
        }
    }

    if (bipartite)
        printf("The graph is bipartite.\n");
    else
        printf("The graph is not bipartite.\n");

    return 0;
}

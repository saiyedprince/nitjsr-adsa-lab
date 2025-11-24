#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000

typedef struct Node {
    int v;
    struct Node* next;
} Node;

Node* adj[MAX]; 
int V;          

void addEdge(int u, int v) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->v = v;
    n->next = adj[u];
    adj[u] = n;
}

int index = 0, sccCount = 0;
int indices[MAX], lowlink[MAX], onStack[MAX];
int stack[MAX], top = -1;

void strongConnect(int v) {
    indices[v] = lowlink[v] = index++;
    stack[++top] = v;
    onStack[v] = 1;

    for (Node* n = adj[v]; n; n = n->next) {
        if (indices[n->v] == -1) {
            strongConnect(n->v);
            lowlink[v] = (lowlink[v] < lowlink[n->v]) ? lowlink[v] : lowlink[n->v];
        } else if (onStack[n->v]) {
            lowlink[v] = (lowlink[v] < indices[n->v]) ? lowlink[v] : indices[n->v];
        }
    }

    if (lowlink[v] == indices[v]) {
        printf("SCC %d: ", ++sccCount);
        int w;
        do {
            w = stack[top--];
            onStack[w] = 0;
            printf("%d ", w);
        } while (w != v);
        printf("\n");
    }
}

void findSCCs() {
    memset(indices, -1, sizeof(indices));
    memset(onStack, 0, sizeof(onStack));
    for (int v = 0; v < V; v++) {
        if (indices[v] == -1)
            strongConnect(v);
    }
}

int timeDFS = 0;
int disc[MAX], low[MAX], parent[MAX];
int ap[MAX]; 

void BCCUtil(int u) {
    static int children = 0;
    disc[u] = low[u] = ++timeDFS;

    for (Node* n = adj[u]; n; n = n->next) {
        int v = n->v;
        if (disc[v] == -1) {
            children++;
            parent[v] = u;
            BCCUtil(v);

            low[u] = (low[u] < low[v]) ? low[u] : low[v];

            if ((parent[u] == -1 && children > 1) || (parent[u] != -1 && low[v] >= disc[u]))
                ap[u] = 1;

            if (low[v] > disc[u])
                printf("Bridge: %d - %d\n", u, v);
        } else if (v != parent[u]) {
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
        }
    }
}

void findBCCs() {
    memset(disc, -1, sizeof(disc));
    memset(low, -1, sizeof(low));
    memset(parent, -1, sizeof(parent));
    memset(ap, 0, sizeof(ap));

    for (int i = 0; i < V; i++) {
        if (disc[i] == -1)
            BCCUtil(i);
    }

    printf("Articulation Points:\n");
    for (int i = 0; i < V; i++) {
        if (ap[i])
            printf("%d ", i);
    }
    printf("\n");
}

int main() {
    V = 7;
    addEdge(0, 1);
    addEdge(1, 2);
    addEdge(2, 0);
    addEdge(1, 3);
    addEdge(3, 4);
    addEdge(4, 5);
    addEdge(5, 3);
    addEdge(5, 6);

    printf("Strongly Connected Components:\n");
    findSCCs();

    memset(adj, 0, sizeof(adj));
    V = 5;
    addEdge(0, 1); addEdge(1, 0);
    addEdge(1, 2); addEdge(2, 1);
    addEdge(2, 0); addEdge(0, 2);
    addEdge(1, 3); addEdge(3, 1);
    addEdge(3, 4); addEdge(4, 3);

    printf("\nBiconnected Components, Articulation Points, and Bridges:\n");
    findBCCs();

    return 0;
}
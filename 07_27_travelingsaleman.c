#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h> 

#define MAX_VERTICES 50
#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList {
    AdjListNode *head;
} AdjList;

typedef struct Graph {
    int V;
    AdjList* array;
} Graph;

AdjListNode* newAdjListNode(int dest) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

int stack[MAX_VERTICES];
int top = -1;

void push(int v) {
    stack[++top] = v;
}

int pop() {
    return stack[top--];
}

void tsp_util(int n, int graph[][n], bool visited[], int currPos, int count, int cost, int* min_cost) {
    if (count == n) {
        if (graph[currPos][0] != 0) {
            *min_cost = MIN(*min_cost, cost + graph[currPos][0]);
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i] && graph[currPos][i] != 0) {
            visited[i] = true;
            tsp_util(n, graph, visited, i, count + 1, cost + graph[currPos][i], min_cost);
            
        
            visited[i] = false;
        }
    }
}

void solve_tsp(int n, int graph[][n]) {
    bool visited[n];
    for(int i = 0; i < n; i++) {
        visited[i] = false;
    }

    visited[0] = true;
    int min_cost = INT_MAX;

    tsp_util(n, graph, visited, 0, 1, 0, &min_cost);

    printf("\n--- Traveling Salesperson Problem ---\n");
    if(min_cost == INT_MAX) {
        printf("No Hamiltonian cycle exists.\n");
    } else {
        printf("Minimum cost for the tour: %d\n", min_cost);
    }
}

int main() {
    int n_tsp = 4;
    int tsp_graph[4][4] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    solve_tsp(n_tsp, tsp_graph);


    return 0;
}
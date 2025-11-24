#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 30 

int graph[MAX_VERTICES][MAX_VERTICES] = {0};
int N; 
int M; 
int E; 

bool is_compatible(int candidate_v, int current_size, int current_clique_indices[]) {
    
    for (int i = 0; i < current_size; i++) {
        int existing_v = current_clique_indices[i];
        
        if (graph[candidate_v][existing_v] == 0) {
            return false;
        }
    }
    return true;
}

bool find_clique_recursive(int start_idx, int current_size, int current_clique_indices[]) {
    
    if (current_size == M) {
        printf("\nFound a clique of size %d: {", M);
        for (int i = 0; i < M; i++) {
           
            printf("%d%s", current_clique_indices[i] + 1, (i < M - 1 ? ", " : ""));
        }
        printf("}\n");
        return true;
    }

    for (int v = start_idx; v < N; v++) {
        if (N - v + current_size < M) {
            return false; 
        }

        if (is_compatible(v, current_size, current_clique_indices)) {
            
            current_clique_indices[current_size] = v;

            if (find_clique_recursive(v + 1, current_size + 1, current_clique_indices)) {
                return true; 
            }

        }
    }
    return false;
}

int main() {
    printf("--- Clique Decision Problem (NP-Complete) ---\n");
    printf("Enter the number of vertices (N, max %d): ", MAX_VERTICES);
    if (scanf("%d", &N) != 1 || N > MAX_VERTICES || N <= 0) {
        printf("Invalid N. Exiting.\n");
        return 1;
    }

    printf("Enter the number of edges (E): ");
    if (scanf("%d", &E) != 1 || E < 0) {
        printf("Invalid E. Exiting.\n");
        return 1;
    }

    printf("Enter the target clique size (M, M <= N): ");
    if (scanf("%d", &M) != 1 || M <= 0 || M > N) {
        printf("Invalid M. Exiting.\n");
        return 1;
    }

    // Input edges
    printf("\nEnter %d edges (format: vertex1 vertex2, e.g., 1 2):\n", E);
    for (int i = 0; i < E; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > N || v < 1 || v > N || u == v) {
            printf("Invalid edge input. Exiting.\n");
            return 1;
        }
        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1;
    }

    printf("\nStarting search for a clique of size %d...\n", M);

    int current_clique[MAX_VERTICES]; 

    if (find_clique_recursive(0, 0, current_clique)) {
        printf("\nRESULT: The graph DOES contain a clique of size %d.\n", M);
    } else {
        printf("\nRESULT: The graph DOES NOT contain a clique of size %d.\n", M);
    }

    return 0;
}
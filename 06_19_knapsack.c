#include <stdio.h>

int maxProfit = 0;

void knapsackBT(int i, int n, int W, int wt[], int val[], int currWeight, int currProfit) {
    if (i == n) {
        if (currWeight <= W && currProfit > maxProfit)
            maxProfit = currProfit;
        return;
    }

    // Include item i
    knapsackBT(i + 1, n, W, wt, val, currWeight + wt[i], currProfit + val[i]);

    // Exclude item i
    knapsackBT(i + 1, n, W, wt, val, currWeight, currProfit);
}

int main() {
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    int W = 50;
    int n = sizeof(val) / sizeof(val[0]);

    knapsackBT(0, n, W, wt, val, 0, 0);
    printf("Maximum value (Backtracking): %d\n", maxProfit);
    return 0;
}
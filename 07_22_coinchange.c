#include <stdio.h>

void coinChangeGreedy(int coins[], int n, int amount) {
    printf("Coins used to make change for %d:\n", amount);
    
    for (int i = n - 1; i >= 0; i--) {
        while (amount >= coins[i]) {
            amount -= coins[i];
            printf("%d ", coins[i]);
        }
    }

    if (amount > 0) {
        printf("\nRemaining amount %d cannot be changed with given denominations.\n", amount);
    } else {
        printf("\nChange successfully made.\n");
    }
}

int main() {
    int coins[] = {1, 2, 5, 10, 20, 50, 100, 200, 500, 2000};
    int n = sizeof(coins) / sizeof(coins[0]);
    
    int amount;
    printf("Enter the amount to change: ");
    scanf("%d", &amount);

    coinChangeGreedy(coins, n, amount);

    return 0;
}
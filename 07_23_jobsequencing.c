#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[10];
    int deadline;
    int profit;
} Job;

int compare(const void *a, const void *b) {
    Job *job1 = (Job *)a;
    Job *job2 = (Job *)b;
    return job2->profit - job1->profit;
}

void jobSequencing(Job jobs[], int n) {
    
    qsort(jobs, n, sizeof(Job), compare);

    int maxDeadline = 0;
    for (int i = 0; i < n; i++) {
        if (jobs[i].deadline > maxDeadline)
            maxDeadline = jobs[i].deadline;
    }

    int slots[maxDeadline];
    char result[maxDeadline][10];
    memset(slots, 0, sizeof(slots));

    int totalProfit = 0;

    for (int i = 0; i < n; i++) {
        for (int j = jobs[i].deadline - 1; j >= 0; j--) {
            if (slots[j] == 0) {
                slots[j] = 1;
                strcpy(result[j], jobs[i].id);
                totalProfit += jobs[i].profit;
                break;
            }
        }
    }

    printf("Scheduled Jobs: ");
    for (int i = 0; i < maxDeadline; i++) {
        if (slots[i])
            printf("%s ", result[i]);
    }
    printf("\nTotal Profit: %d\n", totalProfit);
}

int main() {
    Job jobs[] = {
        {"J1", 2, 100},
        {"J2", 1, 19},
        {"J3", 2, 27},
        {"J4", 1, 25},
        {"J5", 3, 15}
    };
    int n = sizeof(jobs) / sizeof(jobs[0]);

    jobSequencing(jobs, n);

    return 0;
}
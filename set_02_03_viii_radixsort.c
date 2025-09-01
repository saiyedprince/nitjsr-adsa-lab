#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) 
      if(arr[i] > max) 
        max = arr[i];
    return max;
}
void countSortByDigit(int arr[], int n, int exp) {
    int output[100], count[10] = {0};
    for (int i = 0; i < n; i++) 
      count[(arr[i]/exp)%10]++;
    for (int i = 1; i < 10; i++) 
      count[i] += count[i-1];
    for (int i = n-1; i >= 0; i--) {
        int digit = (arr[i]/exp)%10;
        output[count[digit]-1] = arr[i];
        count[digit]--;
    }
    for (int i = 0; i < n; i++) 
      arr[i] = output[i];
}

void radix_sort(int arr[], int n) {
    int max = getMax(arr, n);
    for (int exp = 1; max/exp > 0; exp *= 10)
        countSortByDigit(arr, n, exp);
}

int main() {
    int n;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n]; 

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    radix_sort(arr,n);

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");
    return 0;
}
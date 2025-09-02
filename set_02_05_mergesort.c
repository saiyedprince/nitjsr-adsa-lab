#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++)
      L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) 
      R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1)
       arr[k++] = L[i++];
    while (j < n2)
       arr[k++] = R[j++];
    free(L);
    free(R);
}

void mergeSort(int arr[], int n) {
    int width, i;

    // the size of subarrays to merge
    for (width = 1; width < n; width *= 2) {
        // adjacent subarrays of size 'width'
        for (i = 0; i < n; i += 2 * width) {
            int left = i;
            int mid = i + width - 1;
            int right = (i + 2 * width - 1 < n - 1) ? i + 2 * width - 1 : n - 1;

            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    mergeSort(arr, n);

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
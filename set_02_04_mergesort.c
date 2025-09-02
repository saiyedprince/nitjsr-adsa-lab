#include <stdio.h>
#include <stdlib.h>

#define MAX 1000
typedef struct {
    int left;
    int right;
} Range;
typedef struct {
    Range data[MAX];
    int top;
} Stack;
void push(Stack *s, int left, int right) {
    s->data[++(s->top)].left = left;
    s->data[s->top].right = right;
}
Range pop(Stack *s) {
    return s->data[(s->top)--];
}
int isEmpty(Stack *s) {
    return s->top == -1;
}
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
    Stack s;
    s.top = -1;

    // Dividing and pushing into stack
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n - 1) ? left + 2 * size - 1 : n - 1;
            push(&s, left, right);
        }

        // Conquerering 
        while (!isEmpty(&s)) {
            Range r = pop(&s);
            int mid = (r.left + r.right) / 2;
            merge(arr, r.left, mid, r.right);
        }
    }
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    if (n > MAX) {
        printf("Maximum allowed size is %d\n", MAX);
        return 1;
    }
    int arr[MAX];
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
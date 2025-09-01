#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

void heapify(int a[] , int n , int root){
  int lar = root;
  int ln = 2*root+1;
  int rn= 2*root+2;
  if(ln<n && a[ln]>a[root]){
    lar=ln;
  }
  if(rn<n && a[rn]>a[root]){
    lar=rn;
  }
  if(lar!=root){
    int temp=a[lar];
    a[lar]=a[root];
    a[root]=temp;
    heapify(a , n , lar);
  }
}

void heapsort(int a[] , int n){
  for(int i=n/2-1;i>=0;i--){
    heapify(a,n,i);
  }
  for(int i=n-1;i>0;i--){
    int temp=a[i];
    a[i]=a[0];
    a[0]=temp;
    heapify(a ,i , 0);
  }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
int main() {
    int arr[] = {9, 4, 3, 8, 10, 2, 5}; 
    int n = sizeof(arr) / sizeof(arr[0]);

    heapsort(arr, n);

    printf("Sorted array is \n");
    printArray(arr, n);
    return 0;
}
#include<stdio.h>
#include <stdlib.h>
int main(){
  int r,c;
  printf("enter number of rows ");
  scanf("%d",&r);
  printf("enter number of coloumns ");
  scanf("%d",&c);
  int arr[r][c];
  //intializing array with 0 values
  for (int i=0;i<r;i++){
    for (int j=0;j<c;j++){
      arr[i][j]=0;
    }
  }
  //entering values into the array
  for (int i=0;i<r;i++){
    for (int j=0;j<c;j++){
      printf("enter value in array at position (%d %d) ", i, j);
      scanf("%d",&arr[i][j]);
    }
  }
  //displaying array we created
  for (int i=0;i<r;i++){
    printf("\n");
    for (int j=0;j<c;j++){
      printf("%d ", arr[i][j]);
    }
  }
}
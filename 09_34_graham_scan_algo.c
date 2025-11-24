#include <stdio.h>
#include <stdlib.h>

typedef struct Point {
    int x, y;
} Point;

Point pivot;

void swap(Point *p1, Point *p2) {
    Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
    return val;
}

int distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) +
           (p1.y - p2.y) * (p1.y - p2.y);
}

int compare(const void *vp1, const void *vp2) {
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    int o = orientation(pivot, *p1, *p2);
    if (o == 0)
        return (distSq(pivot, *p1) <= distSq(pivot, *p2)) ? -1 : 1;
    return (o > 0) ? -1 : 1;
}

void grahamScan(Point points[], int n) {
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x)) {
            ymin = points[i].y;
            min = i;
        }
    }

    swap(&points[0], &points[min]);
    pivot = points[0];

    qsort(&points[1], n - 1, sizeof(Point), compare);

    Point hull[n];
    int hullSize = 0;

    for (int i = 0; i < n; i++) {
        while (hullSize >= 2 &&
               orientation(hull[hullSize - 2], hull[hullSize - 1], points[i]) <= 0) {
            hullSize--;
        }
        hull[hullSize++] = points[i];
    }

    printf("Convex Hull:\n");
    for (int i = 0; i < hullSize; i++) {
        printf("(%d, %d)\n", hull[i].x, hull[i].y);
    }
}

int main() {
    int n;
    printf("Enter number of points: ");
    scanf("%d", &n);

    Point points[n];
    printf("Enter points as x y:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &points[i].x, &points[i].y);
    }

    grahamScan(points, n);
    return 0;
}
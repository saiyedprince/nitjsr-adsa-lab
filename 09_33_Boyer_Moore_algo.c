 #include <stdio.h>
#include <string.h>
#include <limits.h>

#define NO_OF_CHARS 256 

void badCharHeuristic(char *str, int size, int badchar[NO_OF_CHARS]) {
    int i;
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1; 

    for (i = 0; i < size; i++)
        badchar[(int) str[i]] = i;  
}

void boyerMooreSearch(char *txt, char *pat) {
    int m = strlen(pat);
    int n = strlen(txt);

    int badchar[NO_OF_CHARS];
    badCharHeuristic(pat, m, badchar);

    int s = 0;  
    while (s <= (n - m)) {
        int j = m - 1;

        while (j >= 0 && pat[j] == txt[s + j])
            j--;

        if (j < 0) {
            printf("Pattern occurs at shift = %d\n", s);

            s += (s + m < n) ? m - badchar[(int)txt[s + m]] : 1;
        } else {
            s += (j - badchar[(int)txt[s + j]] > 1) ? j - badchar[(int)txt[s + j]] : 1;
        }
    }
}

int main() {
    char txt[] = "ABAAABCD";
    char pat[] = "ABC";
    boyerMooreSearch(txt, pat);
    return 0;
}
#include <stdio.h>

#define n 8
int all = (1 << n) - 1;
int count = 0;

void n_queen(int cols, int ld, int rd) {
    if (cols == all)
        count++;
    else {
        int pos = ~(cols | ld | rd) & all;
        while (pos) {
            int p = pos & -pos;
            pos ^= p;
            n_queen(cols | p, (ld | p) << 1, (rd | p) >> 1);
        }
    }
}

int main() {
    n_queen(0, 0, 0);
    printf("%d\n", count);
    return 0;
}

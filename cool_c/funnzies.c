#include <stdio.h>

void print_mult_table(int end) {
    int row = 1, col;

    for (; row <= end; ++row) {
        for (col = 1; col <= end; ++col) {
            printf("%4d", row * col);
        }
        printf("\n");
    }

}

int main() {
    // Problem 1
    print_mult_table(12);

    // Problem 2

}

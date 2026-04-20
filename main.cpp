#include <stdio.h>
#include <time.h>

#include "render.h"

int main() {
    clock_t start, end = 0;
    start = clock();
    Render();
    end = clock();
    fprintf(stderr, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
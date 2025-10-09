#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Usage: %s N [M [S]]\n", argv[0]);
        return 1;
    }
    
    int start, stop, step;
    
    if (argc == 2) {
        start = 0;
        stop = atoi(argv[1]);
        step = 1;
    } else if (argc == 3) {
        start = atoi(argv[1]);
        stop = atoi(argv[2]);
        step = 1;
    } else {
        start = atoi(argv[1]);
        stop = atoi(argv[2]);
        step = atoi(argv[3]);
    }
    
    int current = start;
    while ((step > 0 && current < stop) || (step < 0 && current > stop)) {
        printf("%d\n", current);
        current += step;
    }
    
    return 0;
}

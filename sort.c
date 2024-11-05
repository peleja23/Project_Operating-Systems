#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "my_sort.h"

int main(int argc, char *argv[]) {
    
    int region;
    region = atoi(argv[2]);
    sorter(argv[1], region);

    return 0;
}
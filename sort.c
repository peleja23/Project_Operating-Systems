#include "my_sort.h"

int main(int argc, char *argv[]) {
    
    int region;
    region = atoi(argv[2]);
    sorter(argv[1], region);

    return 0;
}
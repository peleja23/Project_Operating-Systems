#include "my_sort.h"

int main(int argc, char *argv[]) {

    int region;
    int fd;
    region = atoi(argv[2]);
    fd = read_info(argv[1]);
    sorter (fd, region);
    
    return 0;
}
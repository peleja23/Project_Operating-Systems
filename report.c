#include "my_report.h"

int main(int argc, char *argv[]) {

    int nr_region;
    nr_region = atoi(argv[1]);
    exec_stats(nr_region);
    read_stats(nr_region);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "my_stats.h"

int main(int argc, char *argv[]) {

    exec_sort(argv[1], argv[2]);
    get_stats(argv[1], argv[2]);
    write_in_file();

    return 0;
}
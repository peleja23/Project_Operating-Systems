#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "my_stats.h"

int main(int argc, char *argv[]) {
    
    stats(argv[1], argv[2]);

    return 0;
}
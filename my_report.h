#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int exec_stats(int nr_region, char* path);
int read_stats(int nr_region);
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int exec_sort(char *path, char *region_nr);
int get_stats(char *path, char *region_nr);
int write_in_file();
int write_in_stdout();
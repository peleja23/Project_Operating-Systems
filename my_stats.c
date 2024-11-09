#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "my_stats.h"
#include "my_sort.h"

Info_region Info;
region_stats reg_stats;

get_stats(){
        
}
int exec_sort(char *path, char *region_nr){
    int status;
    if(fork() ==0){
        execlp("./sort", "./sort", path, region_nr, NULL);
        perror("execlp");
        _exit(1);    
    }
    wait(&status);
    if(WIFEXITED(status)){
        return 0;
    }
    return 1;
}

int stats(char *path, char *region_nr) {
    int fd;
    int region;

    exec_sort(path, region_nr);
    region = atoi(region_nr);
    reg_stats.region_id = region;
    // Open file in read-write mode
    fd = open(path, O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &Info, sizeof(Info_region));

    // Get the start and end positions for the region's records
    int min_position = 2 + (region - 1) * Info.registos;
    int max_position = min_position + Info.registos - 1;

    close(fd);
    return 0;
}
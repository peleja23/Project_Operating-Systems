#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h> 

typedef struct Info_region {
int total_region;
int registos;
} Info_region;

typedef struct region_stats {
int region_id;
int median;
float average;
int max;
int min;
} region_stats;

int view_regions(char *path, char *region_nr){
    int fd;
    Info_region Info;
    
    fd = open(path, O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &Info, sizeof(Info_region));
    int region = atoi(region_nr);
    int offSet = ((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int));
    int bytes = lseek(fd, offSet, SEEK_SET);
    printf("rmemoria:%d\n",bytes);
    int ola;
    for(int i = 0; i<Info.registos; i++){
        read(fd,&ola,sizeof(int));
        printf("%d:%d\n",i,ola);
    }
    lseek(fd, 0, SEEK_SET);
    
    close(fd);
    
    return 0;
}
int view_stats(char *path){
    int fd;
    region_stats reg_stats;
    
    fd = open(path, O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &reg_stats, sizeof(region_stats));
    printf("region_id: %d\n",reg_stats.region_id);
    printf("median: %d\n",reg_stats.median);
    printf("average: %.5f\n",reg_stats.average);
    printf("max: %d\n",reg_stats.max);
    printf("min: %d\n",reg_stats.min);
    
    return 0;
}

int main(int argc, char *argv[]){
    if(strcmp(argv[1],"-r") == 0){
        view_regions(argv[2], argv[3]);
    }else if(strcmp(argv[1],"-s") == 0){
        view_stats(argv[2]); 
    }
    return 0;
}
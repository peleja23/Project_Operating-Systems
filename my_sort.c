#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include "my_sort.h"

Info_region Info;

void quickSort(low, high){

    if(low<high){



    }
}

int sorter(char *path, int region){
    int fd;
    int low;
    int high;
    int ola;

    fd = open(path, O_RDWR, 0666);
    read(fd, &Info, sizeof(Info_region));

    printf("%d\n",Info.region_nr);
    printf("%d\n",Info.registos);
    for(int i = 0; i<20; i++){
        read(fd,&ola,sizeof(int));
        printf("%d:%d\n",i,ola);
    }
    low = lseek(fd, (region-1)*(Info.registos)+(2*sizeof(int)), SEEK_SET);
    high = lseek(fd, (((Info.registos))*sizeof(int)) -sizeof(int), SEEK_CUR);
    read(fd, &ola,sizeof(int));
    printf("last:%d\n",ola);
    printf("%d,%d \n",low ,high);
    close(fd);
    
    quickSort(low, high);

}
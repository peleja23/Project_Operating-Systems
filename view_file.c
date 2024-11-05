#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 

typedef struct Info_region {
int total_region;
int registos;
} Info_region;

int main(int argc, char *argv[]){
    int fd;
    int blockSize;
    Info_region Info;
    
    fd = open(argv[1], O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &Info, sizeof(Info_region));
    int region = atoi(argv[2]);
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include "my_sort.h"

Info_region Info;

// Partition function for quicksort
int partition(int *array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            // Swap array[i] and array[j]
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

    // Swap array[i + 1] and array[high] (pivot)
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    return i + 1;
}

// QuickSort recursive function
void quickSort(int *array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);

        // Recursively sort elements before and after partition
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

// Function to read a block of integers from the file at the specified offset
int *mySort(int fd, int offSet, int blockSize, int region){
    int *block = (int *)malloc(blockSize * sizeof(int));
    if (block == NULL) {
        perror("Failed to allocate memory for block");
        exit(1);
    }
    lseek(fd, offSet, SEEK_SET);
    read(fd, block, blockSize * sizeof(int));
    quickSort(block, 0, blockSize - 1);
    int end_1 = block[blockSize - 1];
    lseek(fd, offSet, SEEK_SET);
    printf("primeiro bloco\n");
    for(int i = 0; i<blockSize ; i++){
        printf("temp:%d\n",block[i]);
    }
    write(fd, block, blockSize * sizeof(int));
    read(fd, block, blockSize * sizeof(int));
    quickSort(block, 0, blockSize - 1);
    lseek(fd,(((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int)) + (blockSize * sizeof(int))),SEEK_SET);
    printf("segundo bloco\n");
    for(int i = 0; i<blockSize ; i++){
        printf("temp:%d\n",block[i]);
    }
    
    write(fd, block, blockSize * sizeof(int));
    int start2 = block[0];
    if(end_1 > start2){
        lseek(fd,(((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int)) + ((blockSize / 2) * sizeof(int))),SEEK_SET);
        read(fd, block, blockSize * sizeof(int));
        quickSort(block, 0, blockSize - 1);
        lseek(fd,(((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int)) + ((blockSize / 2) * sizeof(int))),SEEK_SET);
        printf("meio dos blocos\n");
        for(int i = 0; i<blockSize ; i++){
            printf("temp:%d\n",block[i]);
        }
        write(fd, block, blockSize * sizeof(int));
        mySort(fd, offSet, blockSize, region);
    }
    free(block);
    return 0;  
}

// Sorter function that reads a block, sorts it, and handles file operations
int sorter(char *path, int region){
    int fd;
    int blockSize;
    
    fd = open(path, O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &Info, sizeof(Info_region));
    blockSize = Info.registos / 2;  // Assuming this is how block size is calculated
    int offSet = ((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int));
    int bytes = lseek(fd, offSet, SEEK_SET);
    printf("rmemoria:%d\n",bytes);
    int ola;
    for(int i = 0; i<20; i++){
        read(fd,&ola,sizeof(int));
        printf("%d:%d\n",i,ola);
    }
    lseek(fd, 0, SEEK_SET);
    mySort(fd, offSet, blockSize, region);
    
    close(fd);
    
    return 0;
}

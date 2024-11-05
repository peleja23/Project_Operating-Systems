#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include "my_sort.h"
/**
*
* @author José Peleja
* @author Nuno Silva
* @author Rui Dias
*
*/
Info_region Info;

// Function to order each part of the array(partition) in the quicksort algorithm
int partition(int *array, int low, int high) {
    // Choose the pivot as the last element of the current section of the array
    int pivot = array[high];
    int i = low - 1;

    // Loop through each element from the start of the partition up to the element before the pivot
    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            // Swap elements at i and j to place the smaller element on the left
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    // Place the pivot in its correct position by swapping it with the element at "i + 1"
    // Now, all elements to the left of the pivot are smaller, and those to the right are larger
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    return i + 1;
}

// Recursive quicksort function to sort the array
// Divides the array into partitions and sorts them around a pivot element
void quickSort(int *array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);

        // Recursively apply quicksort to the left subarray (elements below the pivot)
        quickSort(array, low, pi - 1);
        // Recursively apply quicksort to the right subarray (elements above the pivot)
        quickSort(array, pi + 1, high);
    }
}
void read_block(int fd, int offset, int *block, int blockSize) {

    lseek(fd, offset, SEEK_SET);
    read(fd, block, blockSize * sizeof(int));

}

void write_block(int fd, int offset, int *block, int blockSize) {

    lseek(fd, offset, SEEK_SET);
    printf("next block\n");
    for(int i = 0; i<blockSize ; i++){
        printf("temp:%d\n",block[i]);
    }
    write(fd, block, blockSize * sizeof(int));
}

// Recursive function responsible for dividing in blocks and sorting all 
int *mySort(int fd, int blockSize, int region) {
    int end_1;
    int start2;
    int *block = (int *)malloc(blockSize * sizeof(int));

    int offSet = ((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int));
    read_block(fd, offSet, block, blockSize);
    quickSort(block, 0, blockSize - 1);
    end_1 = block[blockSize - 1];
    write_block(fd, offSet, block, blockSize);

    offSet = ((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int)) + (blockSize * sizeof(int));
    read_block(fd, offSet, block, blockSize);
    quickSort(block, 0, blockSize - 1);
    start2 = block[0];
    write_block(fd, offSet, block, blockSize);

    if(end_1 > start2){

        offSet = ((region - 1) * Info.registos * sizeof(int)) + (2 * sizeof(int)) + ((blockSize / 2) * sizeof(int));
        read_block(fd, offSet, block, blockSize);
        quickSort(block, 0, blockSize - 1);
        write_block(fd, offSet, block, blockSize);

        mySort(fd, blockSize, region);
    }
    free(block);
    return 0;  
}

int sorter(char *path, int region) {
    int fd;
    int blockSize;
    
    fd = open(path, O_RDWR, 0666);

    //Check if the file failed to open
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }

    read(fd, &Info, sizeof(Info_region));
    blockSize = Info.registos / 2; 
    mySort(fd, blockSize, region);
    
    close(fd);

    return 0;
}

#include "def_struct.h"
#include "my_sort.h"

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

// Recursive function responsible for dividing in blocks and sorting all 
int mySort(int fd, int region, int *buf, int blockSize) {
    int values_read = 0;
    int nr_block = (Info.records + blockSize - 1) / blockSize;
    long offSet;
    int blocksize_init = blockSize;
    int matrix[nr_block][2];
    bool ordered;

    do {
        ordered = true;
        values_read = 0;
        blockSize = blocksize_init;
        buf = realloc(buf, blockSize * sizeof(int));
        for (int i = 0; i < nr_block; i++) {
            offSet = (2 * sizeof(int)) + ((region - 1) * Info.records * sizeof(int)) + (i * blockSize * sizeof(int));
            values_read += blockSize;

            if (values_read > Info.records) {
                blockSize -= (values_read - Info.records);
                buf = realloc(buf, blockSize * sizeof(int));
            }

            lseek(fd, offSet, SEEK_SET);
            read(fd, buf, blockSize * sizeof(int));
            quickSort(buf, 0, blockSize - 1);
            matrix[i][0] = buf[0];
            matrix[i][1] = buf[blockSize - 1];
            lseek(fd, offSet, SEEK_SET);
            write(fd, buf, blockSize * sizeof(int));
        }

        for (int i = 0; i < nr_block - 1; i++) {
            if (matrix[i][1] > matrix[i + 1][0]) {
                ordered = false;
                break;
            }
        }

        if (ordered== false) {
            blockSize = blocksize_init;
            values_read = blockSize / 2;
            buf = realloc(buf, blockSize * sizeof(int));

            for (int i = 0; i < nr_block - 1; i++) {
                offSet = ((region - 1) * Info.records * sizeof(int)) +(2 * sizeof(int) + (i * blockSize * sizeof(int)) + ((blockSize / 2) * sizeof(int)));
                values_read += blockSize;

                if (values_read > Info.records) {
                    blockSize -= (values_read - Info.records);
                    buf = realloc(buf, blockSize * sizeof(int));
                }

                lseek(fd, offSet, SEEK_SET);
                read(fd, buf, blockSize * sizeof(int));
                quickSort(buf, 0, blockSize - 1);
                lseek(fd, offSet, SEEK_SET);
                write(fd, buf, blockSize * sizeof(int));
            }
        }

    } while (ordered == false);

    free(buf);
    return 0;
}
int sorter(char *path, int region) {
    int fd;
    int blockSize = 20;
    int *buf = malloc(blockSize * sizeof(int));

    fd = open(path, O_RDWR, 0666);

    //Check if the file failed to open
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }

    read(fd, &Info, sizeof(Info_region));
    mySort(fd, region, buf, blockSize);
    close(fd);

    return 0;
}

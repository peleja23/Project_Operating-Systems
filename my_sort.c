#include "my_sort.h"
#include "def_struct.h"

Info_region Info;

// Function to read an integer from a specific position in the file
int read_int(int fd, int position) {
    int value;
    lseek(fd, position * sizeof(int), SEEK_SET);
    read(fd, &value, sizeof(int));
    return value;
}

// Function to write an integer to a specific position in the file
void write_int(int fd, int position, int value) {
    lseek(fd, position * sizeof(int), SEEK_SET);
    write(fd, &value, sizeof(int));
}

// Partition function for the quicksort algorithm using file-based access
int partition(int fd, int low, int high) {
    // Read the pivot
    int pivot = read_int(fd, high);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        int current = read_int(fd, j);
        if (current <= pivot) {
            i++;
            // Swap values at i and j
            int temp_i = read_int(fd, i);
            write_int(fd, i, current);
            write_int(fd, j, temp_i);
        }
    }

    // Place the pivot in its correct position
    int temp = read_int(fd, i + 1);
    write_int(fd, i + 1, pivot);
    write_int(fd, high, temp);

    return i + 1;
}

// Recursive quicksort function for sorting the file
void quickSort(int fd, int low, int high) {
    if (low < high) {
        int pi = partition(fd, low, high);

        // Recursively apply quicksort the left and right subarrays
        quickSort(fd, low, pi - 1);
        quickSort(fd, pi + 1, high);
    }
}

// Main function to sort the specified region in the file
int sorter (int fd, int region) {
    int records_per_region = Info.records;

    // Get the start and end positions for the region's records
    int start_position = 2 + (region - 1) * records_per_region;
    int end_position = start_position + records_per_region - 1;
    quickSort(fd, start_position, end_position);

    close(fd);
    return 0;
}

// Function to read the header in the file
// Returns the file descriptor to be used in the sorter function
int read_info (char *path) {
    int fd;

    // Open file in read-write mode
    fd = open(path, O_RDWR, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &Info, sizeof(Info_region));

    return fd;
}
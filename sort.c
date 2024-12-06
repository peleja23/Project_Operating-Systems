#include "my_sort.h"

int main(int argc, char *argv[]) {

    // Verifie if the sort as enough arguments to execute
    if (argc < 3) {
        printf("Usage: ./stats file_name number_of_region\n");
        return 1;
    }
    int region;
    // Convert argv[2] to integer
    region = atoi(argv[2]);
    
    sorter(argv[1], region);

    return 0;
}
#include "my_sort.h"

int main(int argc, char *argv[]) {
    char output_buffer[50];
    // Verifie if the sort as enough arguments to execute
    if (argc < 3) {
        int len = snprintf(output_buffer, sizeof(output_buffer), "Usage: ./stats file_name number_of_region\n");
        write(1, output_buffer, len);
        return 1;
    }
    int region;
    // Convert argv[2] to integer
    region = atoi(argv[2]);
    // Call sorter to sort the file
    sorter(argv[1], region);

    return 0;
}
#include "my_sort.h"

int main(int argc, char *argv[]) {

    int region;
    int fd;
    
    //Verifie if the user inserted all the necessary arguments to execute the code
    if (argc < 3) {
        printf("Usage: ./sort file_name number_of_region\n");
        return 1;
    }

    // Call the "read_info" function, to read the header from the file
    fd = read_info(argv[1]);

    region = atoi(argv[2]);
    // Call the "sorter" function, passing the file descriptor and region to sort
    sorter (fd, region);
    
    return 0;
}
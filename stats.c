#include "my_stats.h"

int main(int argc, char *argv[]) {
    char output_buffer[50];
    //Verifie if the user inserted all the necessary arguments to execute the code
    if (argc < 4) {
        int len = snprintf(output_buffer, sizeof(output_buffer), "Usage: ./stats file_name number_of_region option\n");
        write(1, output_buffer, len);
        return 1;
    }
    // Call the execution of sort
    exec_sort(argv[1], argv[2]);
    
    // Get stats for the given region
    get_stats(argv[1], argv[2]);

    // CHeck the flag and decide if it should write in file or write in Stdout
    if(strcmp("-f",argv[3]) == 0){
        write_in_file();
    }
    if(strcmp("-d",argv[3]) == 0){
        write_in_stdout();
    }
    return 0;
}
#include "my_stats.h"

int main(int argc, char *argv[]) {
    
    //Verifie if the user inserted all the necessary arguments to execute the code
    if (argc < 4) {
        printf("Usage: ./stats file_name number_of_region option\n");
        return 1;
    }
    if(exec_sort(argv[1], argv[2]) > 0){
        perror("exec_sort");
        return 1;
    }
    get_stats(argv[1], argv[2]);

    if(strcmp("-f",argv[3]) == 0){
        write_in_file();
    }
    if(strcmp("-d",argv[3]) == 0){
        write_in_stdout();
    }
    return 0;
}
#include "my_stats.h"

int main(int argc, char *argv[]) {
    
    exec_sort(argv[1], argv[2]);
    get_stats(argv[1], argv[2]);

    if(strcmp("-f",argv[3]) == 0){
        write_in_file();
    }
    if(strcmp("-d",argv[3]) == 0){
        write_in_file();
    }
    return 0;
}
#include "my_report.h"

int main(int argc, char *argv[]) {
    
    int nr_region;
    nr_region = atoi(argv[1]);
    if(strcmp("-f",argv[3]) == 0){
        exec_stats(nr_region, argv[2]);
        read_stats(nr_region);
    }
    if(strcmp("-d",argv[3]) == 0){
        exec_stats(nr_region, argv[2]);
        read_stats(nr_region);
    }    
    return 0;
}
#include "my_stats.h"
#include "def_struct.h"

Info_region Info;
region_stats reg_stats;

int exec_sort(char *path, char *region_nr){
    int status;
    if(fork() == 0){
        execlp("./sort", "./sort", path, region_nr, NULL);
        perror("execlp");
        _exit(1);    
    }
    wait(&status);
    if(WIFEXITED(status) && WEXITSTATUS(status) != 1){
        return 0;
    }
    return 1;
}

void construct_filename(char *buffer, int region_id) {
    const char *prefix = "./regions_stats/region-";
    int i;
    for (i = 0; prefix[i] != '\0'; i++) {
        buffer[i] = prefix[i];
    }

    buffer[i++] = '0' + (region_id / 100) % 10;
    buffer[i++] = '0' + (region_id / 10) % 10;
    buffer[i++] = '0' + region_id % 10;

    const char *suffix = "-stats.bin";
    for (int j = 0; suffix[j] != '\0'; j++, i++) {
        buffer[i] = suffix[j];
    }
    buffer[i] = '\0';
}

int write_in_file(){
    int fd;
    char output_file[50];
    construct_filename(output_file, reg_stats.region_id);

    fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    write(fd, &reg_stats, sizeof(region_stats));
    return 0;
}

int write_in_stdout(){

    write(1, &reg_stats, sizeof(region_stats));
    return 0;
    
}

int get_stats(char *path, char *region_nr){
    int fd;
    int value;
    float average;

    reg_stats.region_id = atoi(region_nr);;
    // Open file in read-write mode
    fd = open(path, O_RDONLY, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    read(fd, &Info, sizeof(Info_region));

    int min_position = (2 + (reg_stats.region_id - 1) * Info.records) * sizeof(int);
    int mid_position = min_position + (((Info.records/2) - 1) * sizeof(int)); 

    lseek(fd, min_position, SEEK_SET);
    for(int i = 0; i<Info.records; i++){
        read(fd, &value, sizeof(int));
        if( i == 0 ){
            reg_stats.min = value;
        }else if( i == (Info.records - 1)){
            reg_stats.max = value;
        }
        average = average + value;
    }
    reg_stats.average = (float)(average / Info.records);
    
    lseek(fd, mid_position, SEEK_SET);
    if((Info.records % 2) == 0){
        int value1;
        read(fd, &value, sizeof(int));
        read(fd, &value1, sizeof(int));
        reg_stats.median = (value + value1)/2;
    }else{
        lseek(fd, sizeof(int), SEEK_CUR);
        read(fd, &value, sizeof(int));
        reg_stats.median = value;
    }

    return 0;
}


#include "my_report.h"
#include "def_struct.h"

region_stats reg_stats;

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

int exec_stats(int nr_region){
    int status;
    char value[4];
    for(int i = 0; i < nr_region; i++){
        if(fork() == 0) {
            snprintf(value, sizeof(value), "%d", i+1);
            execlp("./stats", "./stats", "./generate_sensor_data/sensor_teste.bin", value, NULL);
            perror("execlp");
            _exit(1);
        }
    }
    for(int i = 0; i < nr_region; i++){
        wait(&status);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("stats worked with return value:%d\n", WEXITSTATUS(status));
        }else{
            return 1;
        }
    }
    return 0;
}

int read_stats(int nr_region){
    int status;
    int pd[2];
    pipe(pd);
    struct region_stats all_regs[nr_region];
    
    for(int i = 0; i < nr_region; i++){
        if(fork() == 0) {
            int fd;
            char output_file[50];        
            construct_filename(output_file, i+1);
            fd = open(output_file, O_RDONLY, 0666);
            close(pd[0]);
            read(fd, &reg_stats, sizeof(region_stats));
            write(pd[1], &reg_stats, sizeof(region_stats));
            close(pd[1]);
            _exit(0);
        }
    }
    
    close(pd[1]);
    for(int i = 0; i < nr_region; i++){
        read(pd[0], &all_regs[i], sizeof(region_stats));
        printf("numero da regiao: %d \n", all_regs[i].region_id);
        printf("max: %d \n", all_regs[i].max);
        wait(&status);
        if(WIFEXITED(status)) {
            
        }
        //return 1;
    }
    close(pd[0]);
    return 0;
}
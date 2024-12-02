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

int compare_stats(struct region_stats* all_regs, int nr_region){
    int pos_min = 0;
    int pos_max = 0;
    int pos_min_ave = 0;
    int pos_max_ave = 0;
    int len;
    char buffer[256];
    
    for(int i = 0; i< nr_region; i++){
        if(all_regs[pos_min].min > all_regs[i].min){
            pos_min = i;
        }
        if(all_regs[pos_max].max < all_regs[i].max){
            pos_max = i;
        }
        if(all_regs[pos_min_ave].average > all_regs[i].average){
            pos_min_ave = i;
        }
        if(all_regs[pos_max_ave].average < all_regs[i].average){
            pos_max_ave = i;
        }
    }
    len = snprintf(buffer, sizeof(buffer), "A região %d registou o valor mais alto: %d ºC;\n", all_regs[pos_max].region_id, all_regs[pos_max].max);
    write(1, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "A região %d registou o valor mais baixo: %d ºC;\n", all_regs[pos_min].region_id, all_regs[pos_min].min);
    write(1, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "A região %d registou o valor medio maximo: %.2f ºC;\n", all_regs[pos_max_ave].region_id, all_regs[pos_max_ave].average);
    write(1, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "A região %d registou o valor medio minimo: %.2f ºC;\n\n", all_regs[pos_min_ave].region_id, all_regs[pos_min_ave].average);
    write(1, buffer, len);

    for (int i = 0; i < nr_region; i++) {
        len = snprintf(buffer, sizeof(buffer), "A região %d: \n", all_regs[i].region_id);
        write(1, buffer, len);

        len = snprintf(buffer, sizeof(buffer), "    * Valor medio: %.2f ºC\n", all_regs[i].average);
        write(1, buffer, len);

        len = snprintf(buffer, sizeof(buffer), "    * Mediana: %d ºC\n", all_regs[i].median);
        write(1, buffer, len);

        len = snprintf(buffer, sizeof(buffer), "    * Valor maximo: %d ºC\n", all_regs[i].max);
        write(1, buffer, len);

        len = snprintf(buffer, sizeof(buffer), "    * Valor minimo: %d ºC\n\n", all_regs[i].min);
        write(1, buffer, len);
    }
    return 0;
}

int exec_stats(int nr_region, char* path){
    int status;
    char value[4];
    for(int i = 0; i < nr_region; i++){
        if(fork() == 0) {
            snprintf(value, sizeof(value), "%d", i+1);
            execlp("./stats", "./stats", path, value, "-f", NULL);
            perror("execlp");
            _exit(1);
        }
    }
    for(int i = 0; i < nr_region; i++){
        wait(&status);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            //printf("stats worked with return value:%d\n", WEXITSTATUS(status));
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
        if(fork() == 0){
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
        wait(&status);
        read(pd[0], &reg_stats, sizeof(region_stats));
        all_regs[reg_stats.region_id - 1] = reg_stats;
        if(!(WIFEXITED(status) && WEXITSTATUS(status) == 0)) {
            return 1;
        }   
    }
    close(pd[0]);
    compare_stats(all_regs, nr_region);

    return 0;
}

int exec_stats_stdin(int nr_region, char* path){
    int status;
    char value[4];
    int pd[2];
    pipe(pd);
    struct region_stats all_regs[nr_region];

    for(int i = 0; i < nr_region; i++){
        if(fork() == 0){
            close(pd[0]);
            dup2(pd[1],1);
            close(pd[1]);
            snprintf(value, sizeof(value), "%d", i+1);
            execlp("./stats", "./stats", path, value, "-d", NULL);
            perror("execlp");
            _exit(1);
        }
    }
    close(pd[1]);
    for(int i = 0; i < nr_region; i++){
        wait(&status);
        read(pd[0], &reg_stats, sizeof(region_stats));
        all_regs[reg_stats.region_id - 1] = reg_stats;
        if(!(WIFEXITED(status) && WEXITSTATUS(status) == 0)) {
            return 1;
        }       
    }
    close(pd[0]);
    compare_stats(all_regs, nr_region);
    return 0;
}
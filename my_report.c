#include "my_report.h"
#include "def_struct.h"

region_stats reg_stats;

// Function to create the right name for the temporary file
void construct_filename(char *buffer, int buffer_size, int region_id) {
    snprintf(buffer, buffer_size, "./regions_stats/region-%03d-stats.bin", region_id);
}

// Function to compare and print stats for all the regions
int compare_stats(struct region_stats* all_regs, int nr_region){
    int pos_min = 0;
    int pos_max = 0;
    int pos_min_ave = 0;
    int pos_max_ave = 0;
    int len;
    char buffer[256];
    
    //Compare all the stats to get the position of minimum, maximium , minimum average and maximum average
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

    // Cicle to print all the stats to all the regions
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

// Function to execute the stats program when the flag is for the file
int exec_stats(int nr_region, char* path, int max_processes){
    char value[4];
    int counter_region = 0;
    int counter_active = 0;
    // Cicle to create child processes until the total regions 
    while(counter_region < nr_region){
        // Loop to create child processes up to the allowed m aximum
        while(counter_active < max_processes){
            if(fork() == 0) {
                // Convert the current region number to a string
                snprintf(value, sizeof(value), "%d", counter_region +1);
                execlp("./stats", "./stats", path, value, "-f", NULL);
                perror("execlp");
                _exit(1);
            } 
            counter_region ++;
            counter_active ++;
        }
        // Wait for one child process to finish if the maximum active processes is reached
        if(counter_active > 0){ 
            wait(NULL);
            counter_active --;
        }
    }
    // Wait for the remaining childs to finish
    for(int i = 0; i < counter_active; i++){
        wait(NULL);
    }
    return 0;
}

// Functions to read statistics from the files 
int read_stats(int nr_region){
    // Array to store statistics struct of all regions
    struct region_stats all_regs[nr_region];
    int pd[2];
    if (pipe(pd) == -1) {
        perror("pipe");
        return 1; 
    }
    // Loop to create a child process for each region    
    for(int i = 0; i < nr_region; i++){
        if(fork() == 0){
            int fd;
            char output_file[50];
            // Construct the file name for the current region      
            construct_filename(output_file, 50, i+1);
            fd = open(output_file, O_RDONLY, 0666);
            if(fd < 0){
                perror("open");
                return 1;
            }
            close(pd[0]);
            // Read the statistics from the file into the reg_stats structure
            read(fd, &reg_stats, sizeof(region_stats));
            close(fd);
            // Write the region statistics to the pipe
            write(pd[1], &reg_stats, sizeof(region_stats));
            close(pd[1]);
            _exit(0);
        }
    }
    
    close(pd[1]);
    // Parent waits for all child processes and reads their results from pipe
    for(int i = 0; i < nr_region; i++){
        wait(NULL);
        read(pd[0], &reg_stats, sizeof(region_stats));
        // Store the statistics in the correct position of the all_regs array
        all_regs[reg_stats.region_id - 1] = reg_stats;
    }
    close(pd[0]);
    // Compare and print the statistics across all regions
    compare_stats(all_regs, nr_region);

    return 0;
}

// Function to execute the stats program using Stdout and reading from Stdin
int exec_stats_stdin(int nr_region, char* path){
    int status;
    char value[4];
    // Array to store statistics struct of all regions
    struct region_stats all_regs[nr_region];
    int pd[2];
    if (pipe(pd) == -1) {
        perror("pipe");
        return 1; 
    }
    // Loop to create a child process for each region
    for(int i = 0; i < nr_region; i++){
        if(fork() == 0){
            close(pd[0]);
            // Redirect stdout to the write end of the pipe and with that stats writes to the pipe
            dup2(pd[1],1);
            close(pd[1]);
            // Convert the current region number to a string
            snprintf(value, sizeof(value), "%d", i+1);
            execlp("./stats", "./stats", path, value, "-d", NULL);
            perror("execlp");
            _exit(1);
        }
    }
    close(pd[1]);
    // Parent waits for all child processes and reads their results from pipe
    for(int i = 0; i < nr_region; i++){
        wait(&status);
        // Read the structs with statistics from the pipe
        read(pd[0], &reg_stats, sizeof(region_stats));
        all_regs[reg_stats.region_id - 1] = reg_stats;
        if(!(WIFEXITED(status) || WEXITSTATUS(status) == 0)) {
            return 1;
        }       
    }
    close(pd[0]);
    // Compare and print the statistics across all regions
    compare_stats(all_regs, nr_region);
    return 0;
}
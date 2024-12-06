#include "my_report.h"

int main(int argc, char *argv[]) {
    int nr_region, execution_time;
    char output_buffer[50];
    time_t start, end;

    // Save the start time of the program
    start = time(NULL);

    nr_region = atoi(argv[1]);

    // CHeck the flag and decide if it should read from the file or read from Stdout
    if(strcmp("-f",argv[3]) == 0){
        int max_processes = atoi(argv[4]);
        // Call the function to execute the program stats based on file input
        exec_stats(nr_region, argv[2], max_processes);
        
        // Call the function to execute the program stats based on file input
        read_stats(nr_region);
    }
    
    if(strcmp("-d",argv[3]) == 0){
        // Call the function to execute the program stats based on Stdout and read from Stdin
        exec_stats_stdin(nr_region, argv[2]);
    }    

    // Save the finish time of the program
    end = time(NULL);
    // Calculate the total execution time
    execution_time = difftime(end, start);
    int len = snprintf(output_buffer, sizeof(output_buffer), "Total Execution Time: %d seconds\n", execution_time);
    write(1, output_buffer, len);

    return 0;
}
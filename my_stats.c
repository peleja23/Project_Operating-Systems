#include "my_stats.h"
#include "def_struct.h"

Info_region Info;
region_stats reg_stats;

// Function to create the right name for the temporary file
void construct_filename(char *buffer, int buffer_size, int region_id) {
    snprintf(buffer, buffer_size, "./regions_stats/region-%03d-stats.bin", region_id);
}

// Function to write the stats struck to the respective file
int write_in_file(){
    int fd;
    char output_file[50];

    //Build the string to open the correct file 
    construct_filename(output_file, 50, reg_stats.region_id);
    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    // Write the struct to the file
    write(fd, &reg_stats, sizeof(region_stats));
    close(fd);
    return 0;
}

// Function to write the stats struck to the Stdout
int write_in_stdout(){
    write(1, &reg_stats, sizeof(region_stats));
    return 0;
    
}

// Function to execute the sort program 
int exec_sort(char *path, char *region_nr){
    // Create a child process to execute sort
    if(fork() == 0){
        execlp("./sort", "./sort", path, region_nr, NULL);
        perror("execlp");
        _exit(1);    
    }
    wait(NULL);
    return 1;
}
// Function to calculate the stats
int get_stats(char *path, char *region_nr){
    int fd;
    int value;
    float average;
    // Amount of ints to read from the file
    int BUF_SIZE = 128;
    int buf[BUF_SIZE];
    int values_read = 0;
    int bytes_read = 0;
    int to_read = 0;
    int counter = 0;

    reg_stats.region_id = atoi(region_nr);
    // Open file in read-write mode
    fd = open(path, O_RDONLY, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return -1;
    }
    // Read the header of the file
    read(fd, &Info, sizeof(Info_region));
    // Positions inside the file to later do lseek
    int min_position = (2 + (reg_stats.region_id - 1) * Info.records) * sizeof(int);
    int mid_position = min_position + (((Info.records/2) - 1) * sizeof(int)); 
    lseek(fd, min_position, SEEK_SET);

    // Loop to read integer records from the file and process statistics
    while(values_read < Info.records){
        // Calculate how many records to read in this iteration
        to_read =  Info.records - values_read;
        // Limit the number of records to read to the buffer size (BUF_SIZE)
        if (to_read > BUF_SIZE){
            to_read = BUF_SIZE;
        }
        // Read a chunk of integers from the file into the buffer
        bytes_read = read (fd, &buf, to_read * sizeof(int));
        // Update the ammount of values read
        values_read = values_read + (bytes_read/sizeof(int));
        // Cicle to calculate the average, minimum and maximum values
        for(int i = 0; i<to_read; i++){
            value = buf[i];
            if( counter == 0 ){
                reg_stats.min = value;
            }else if( counter == Info.records - 1){
                reg_stats.max = value;
            }
            average = average + value;
            // Counter to help getting the minimum and maximum values
            counter ++;
        }
    }
    // Calculate the average value
    reg_stats.average = (float)(average / Info.records);

    // Set the file offset to the midpoint to calculate the median
    lseek(fd, mid_position, SEEK_SET);

    // Check if the number of records is even
    if((Info.records % 2) == 0){
        int value1;
        read(fd, &value, sizeof(int));
        read(fd, &value1, sizeof(int));
        reg_stats.median = (value + value1)/2;
    }else{
        // Skip one integer forward to reach the middle for odd records
        lseek(fd, sizeof(int), SEEK_CUR);
        read(fd, &value, sizeof(int));
        reg_stats.median = value;
    }
    close(fd);

    return 0;
}


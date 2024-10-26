#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "generate_sensor_data.h"

int generate_temperature() {
    return rand() % 81 - 30;
}

int generate_temperature_data(int num_regions, int num_records, const char *output_file) {
    srand(time(NULL));

    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening output file");
        return 1;
    }

    if (write(fd, &num_regions, sizeof(int)) == -1 || write(fd, &num_records, sizeof(int)) == -1) {
        perror("Error writing header to file");
        close(fd);
        return 1;
    }

    for (int i = 0; i < num_regions; i++) {
        for (int j = 0; j < num_records; j++) {
            int temp = generate_temperature();
            if (write(fd, &temp, sizeof(int)) == -1) {
                perror("Error writing temperature data");
                close(fd);
                return 1;
            }
        }
    }

    if (close(fd) == -1) {
        perror("Error closing the file");
        return 1;
    }

    return 0;
}


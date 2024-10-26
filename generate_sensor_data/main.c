#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "generate_sensor_data.h"

int main(int argc, char * argv[]) {
    if (argc != 4) {
        printf("Usage: %s <num_regions> <num_records> <output_file>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    int num_regions = atoi(argv[1]);
    int num_records = atoi(argv[2]);
    const char *output_file = argv[3];

    // Generate temperature data
    if (generate_temperature_data(num_regions, num_records, output_file) == 0) {
        printf("Temperature data generated successfully in file: %s\n", output_file);
    } else {
        printf("Error occurred during data generation.\n");
        return 1;
    }

    return 0;
}

//Struct to save the header of the file
typedef struct info_region {
int total_region;
int records;
} Info_region;

//Struct to save the statistics of a certain region of the file
typedef struct region_stats {
int region_id;
int median;
float average;
int max;
int min;
} region_stats;
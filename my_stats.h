typedef struct region_stats {
int region_id;
int median;
float average;
int max;
int min;
} region_stats;

int stats(char *path, int region);
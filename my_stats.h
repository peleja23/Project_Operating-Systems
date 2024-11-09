typedef struct region_stats {
int region_id;
int median;
float average;
int max;
int min;
} region_stats;

int exec_sort(char *path, char *region_nr);
int get_stats(char *path, char *region_nr);
#ifndef __LEVELS_H__
#define __LEVELS_H__

/* Simple arrays that hold the amount of cells of each type to spawn */

#define TOTAL_LEVELS 6

int blood_cell_counts[TOTAL_LEVELS] =   {4, 2, 5, 6, 10, 3};

int virus1_count[TOTAL_LEVELS] =        {2, 2, 4, 6, 0, 10};
int virus2_count[TOTAL_LEVELS] =        {0, 2, 2, 6, 0, 10};
int virus3_count[TOTAL_LEVELS] =        {0, 0, 1, 6, 4, 10};

#endif /* __LEVELS_H__ */
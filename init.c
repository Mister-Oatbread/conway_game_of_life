

/**
 * Conway's game of life
 * Init file to wipe/produce the csv for initial condition
 *
 * Author: Mister Oatbread
 */

#include "gol.h"
#include <string.h>

int main(void) {

    // craft one row
    char * entry = malloc(SIZE_ENTRY);
    memcpy(entry, "0,", SIZE_ENTRY);
    char * line = malloc(SIZE_ENTRY*NUMBER_OF_COLUMNS + 1);

    for (short column = 0; column < NUMBER_OF_COLUMNS; column++) {
        memcpy(line + column*SIZE_ENTRY, entry, SIZE_ENTRY);
    }

    line[NUMBER_OF_COLUMNS*SIZE_ENTRY] = '\0';

    // fill csv with crafted rows
    FILE *initial_condition = fopen("initial_condition.csv", "w+");

    for (short row = 0; row < NUMBER_OF_ROWS; row++) {
        fprintf(initial_condition, "%s\n", line);
    }

    printf("csv created successfully\n");
    fclose(initial_condition);

    // return zero
    return 0;
}



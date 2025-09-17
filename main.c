

/**
 * Conway's game of life
 *
 * Author: Mister Oatbread
 */

#include <stdio.h>
#include <stdbool.h>
#include <locale.h>

// cool macros
#define ACTIVE true
#define INACTIVE false

#define NUMBER_OF_COLUMNS 100
#define NUMBER_OF_ROWS 50
#define STATE_SIZE NUMBER_OF_COLUMNS*NUMBER_OF_ROWS // 100*50 = 5000
#define FULL "■"
#define EMPTY "□"

// prototyping
void modify_cell(bool *state, const short x_coordinate, const short y_coordinate, const bool operation);
void print_state(bool *state);

int main(void) {

    setlocale(LC_ALL, "");
    // create 150 x 50 block of cells (STATE_SIZE) in total
    printf("Conway's game of life by Mister Oatbread\n\n");
    bool state[STATE_SIZE] = {false};

    modify_cell(state, 20, 20, ACTIVE);
    print_state(state);
    return 0;
}

/**
 * This function lets you activate or deactivate a cell at specific x and y coordinates.
 * The cell in the top left corner is 1,1; the cell in the bottom right corner is NUMBER_OF_COLUMNS,NUMBER_OF_ROWS
 * (may be 150,50; no guarantees).
 */
void modify_cell(bool *state, const short x_coordinate, const short y_coordinate, const bool operation) {
    bool x_coordinate_inside_bounds = (x_coordinate>=1) && (x_coordinate<=NUMBER_OF_COLUMNS);
    bool y_coordinate_inside_bounds = (y_coordinate>=1) && (y_coordinate<=NUMBER_OF_ROWS);

    if (x_coordinate_inside_bounds && y_coordinate_inside_bounds) {
        int address = (x_coordinate-1) + NUMBER_OF_COLUMNS*(y_coordinate-1);
        *(state+address) = operation;
    } else {
        printf("dingus\n");
    }
}

/**
 * Iterate through all state elements and print an
 */
void print_state(bool *state) {
    // TODO: change this to not delete a bunch of stuff at the first iteration
    for (int index=0; index<NUMBER_OF_ROWS; index++) {
        printf("\033[A\033[2K");
    }

    for (int index=0; index<STATE_SIZE; index++) {
        // check what to print
        if (*(state+index) == ACTIVE) {
            printf("%s ", FULL);
        } else {
            printf("%s ", EMPTY);
        }

        // at the end of row, insert linebreak
        if (index%NUMBER_OF_COLUMNS == NUMBER_OF_COLUMNS-1) {
            printf("\n");
        }
    }
}



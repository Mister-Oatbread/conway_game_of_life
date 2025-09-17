

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

// this index tracks how many lines have been actively written by the program
// and can be used to delete as many
short number_of_lines_printed = 0;

// prototyping
void set_cell_status(bool *state, const short x_coordinate, const short y_coordinate, const bool operation);
bool read_cell(bool *state, const short x_coordinate, const short y_coordinate);
int calculate_index_with_coordinates(const short x_coordinate, const short y_coordinate);
void print_state(bool *state);

int main(void) {

    setlocale(LC_ALL, "");
    // create 150 x 50 block of cells (STATE_SIZE) in total
    printf("Conway's game of life by Mister Oatbread\n\n");
    bool state[STATE_SIZE] = {false};

    set_cell_status(state, 20, 20, ACTIVE);
    set_cell_status(state, 21, 21, ACTIVE);
    set_cell_status(state, 300, 200, INACTIVE);
    print_state(state);
    printf("%B\n", read_cell(state, 20, 20));
    printf("%B\n", read_cell(state, 21, 20));
    printf("%B\n", read_cell(state, 200, 200));
    return 0;
}

/**
 * This function lets you activate or deactivate a cell at specific x and y coordinates.
 * The cell in the top left corner is 1,1; the cell in the bottom right corner is NUMBER_OF_COLUMNS,NUMBER_OF_ROWS
 * (may be 100,50; no guarantees).
 */
void set_cell_status(bool *state, const short x_coordinate, const short y_coordinate, const bool operation) {
    int index = calculate_index_with_coordinates(x_coordinate, y_coordinate);
    if (index != -1) {
        *(state+index) = operation;
    } else {
        printf("could not find cell to modify dingus\n");
    }
}

/**
 * This function returns the state of a cell with given x and y coordinates
 * If the coordinates are out of bounds, false is returned
 */
bool read_cell(bool *state, const short x_coordinate, const short y_coordinate) {
    bool cell_value;
    int index = calculate_index_with_coordinates(x_coordinate, y_coordinate);
    if (index != -1) {
        cell_value = *(state+index);
    } else {
        printf("could not find cell to read dingus\n");
        cell_value = INACTIVE;
    }
    return cell_value;
}

/**
 * takes in coordinates given the convention and returns the corresponding index in the state array,
 * or -1 if the coordinates are out of bounds
 */
int calculate_index_with_coordinates(const short x_coordinate, const short y_coordinate) {
    int address;
    bool x_coordinate_inside_bounds = (x_coordinate>=1) && (x_coordinate<=NUMBER_OF_COLUMNS);
    bool y_coordinate_inside_bounds = (y_coordinate>=1) && (y_coordinate<=NUMBER_OF_ROWS);

    if (x_coordinate_inside_bounds && y_coordinate_inside_bounds) {
        address = (x_coordinate-1) + NUMBER_OF_COLUMNS*(y_coordinate-1);
    } else {
        address = -1;
    }
    return address;
}

/**
 * Iterate through all state elements and print an
 */
void print_state(bool *state) {
    // TODO: change this to not delete a bunch of stuff at the first iteration
    for (int index=0; index<number_of_lines_printed; index++) {
        printf("\033[A\033[2K");
    }
    number_of_lines_printed = 0;

    for (int index=0; index<STATE_SIZE; index++) {
        // check what to print, and add spacer for more visual consisteny between rows and columns
        if (*(state+index) == ACTIVE) {
            printf("%s ", FULL);
        } else {
            printf("%s ", EMPTY);
        }

        // at the end of row, insert linebreak
        if (index%NUMBER_OF_COLUMNS == NUMBER_OF_COLUMNS-1) {
            printf("\n");
            number_of_lines_printed++;
        }
    }
}



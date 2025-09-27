

/**
 * Conway's game of life
 * Main File with all the functionality
 *
 * Author: Mister Oatbread
 */

#include <complex.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include "gol.h"

short number_of_printed_rows = 0;
bool line_delection_active = true;

int main(void) {

    setlocale(LC_ALL, "");
    // create 150 x 50 block of cells (STATE_SIZE) in total
    printf("Conway's game of life by Mister Oatbread\n\n");

    bool state[STATE_SIZE] = {false};
    bool next_state[STATE_SIZE] = {false};
    apply_initial_condition(state);

    print_state(state);

    // set up time, for now this precise to one second
    clock_t current_time, last_time;
    current_time = clock();
    last_time = current_time;

    // main loop
    for (;;) {
        current_time = clock();
        if (current_time> last_time + REFRESH_RATE*CLOCKS_PER_SEC/1000) {
            last_time = clock();
            // TODO: use cooler time
            update_all_cells(state, next_state);
            write_cell_state_from_to(next_state, state);
            print_state(state);
        }
    }

    // return zero
    return 0;
}

/**
 * this function takes in the "initial_condition.csv" file and writes this initial condition
 * to the initial state.
 */
void apply_initial_condition(bool *state) {
    FILE *initial_condition = fopen("initial_condition.csv", "r");
    short line_size = NUMBER_OF_COLUMNS*SIZE_ENTRY + 2;
    char line[line_size];
    int index = 0;
    int n;
    char * token;

    for (short row = 1; row <= NUMBER_OF_ROWS; row++) {
        fgets(line, line_size, initial_condition);
        token = strtok(line, ",");

        for (short column = 1; column <= NUMBER_OF_COLUMNS; column++) {
            index = calculate_index_with_coordinates(column, row);
            n = atoi(token);
            if (n==1) {
                *(state+index) = ACTIVE;
            } else {
                *(state+index) = INACTIVE;
            }

            if (column < NUMBER_OF_COLUMNS) {
                token = strtok(NULL, ",");
            }
        }
    }
    fclose(initial_condition);
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
 * this function performs a deep copy to write one cell array to another
 */
void write_cell_state_from_to(bool *next_state, bool *state) {
    for (int i=0; i<STATE_SIZE; i++) {
        *(state+i) = *(next_state+i);
    }
}

/**
 * This function returns the state of a cell with given x and y coordinates
 * If the coordinates are out of bounds, false is returned
 */
bool cell_is_active(bool *state, const short x_coordinate, const short y_coordinate) {
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
 * returns true if cell is inside bounds, otherwise returns false.
 */
bool cell_inside_bounds(const short x_coordinate, const short y_coordinate) {
    bool x_coordinate_inside_bounds = (x_coordinate>=1) && (x_coordinate<=NUMBER_OF_COLUMNS);
    bool y_coordinate_inside_bounds = (y_coordinate>=1) && (y_coordinate<=NUMBER_OF_ROWS);
    return x_coordinate_inside_bounds && y_coordinate_inside_bounds;
}

/**
 * takes in coordinates given the convention and returns the corresponding index in the state array,
 * or -1 if the coordinates are out of bounds.
 * It is recommended to catch out of bounds beforehand.
 */
int calculate_index_with_coordinates(const short x_coordinate, const short y_coordinate) {
    int address;
    bool cell_is_inside_bounds = cell_inside_bounds(x_coordinate, y_coordinate);

    if (cell_is_inside_bounds) {
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
    if (line_delection_active) {
        for (int index=0; index<number_of_printed_rows; index++) {
            printf("\033[A\033[2K");
        }
        number_of_printed_rows = 0;
    }

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
            number_of_printed_rows++;
        }
    }
}

/**
 * This function can be called to update the cell matrix
 * To evaluate which cells should live or die, use state.
 * To write the decision, use next_state.
 * Later copy everything
 */
void update_all_cells(bool *state, bool *next_state) {

    int index;
    short number_of_active_neighbours;
    bool *p_current_cell;

    for (short x_coordinate = 1; x_coordinate <= NUMBER_OF_COLUMNS; x_coordinate++) {
        for (short y_coordinate = 1; y_coordinate <= NUMBER_OF_ROWS; y_coordinate++) {
            index = calculate_index_with_coordinates(x_coordinate, y_coordinate);

            // catch cell that is out of bounds, which should not happen
            if (index == -1) {
                printf("updating cells found something out of bounds dingus\n");
                continue;
            } else {
                number_of_active_neighbours = get_number_of_active_neighbours(state, x_coordinate, y_coordinate);
            }

            p_current_cell = next_state+index;

            // decide if cell should be alive or dead in next step
            // under 2 -> dead
            // 2 or 3 -> alive
            // over 3 -> dead
            // exactly 3 from neighbour -> alive
            if (number_of_active_neighbours < 2) {
                *p_current_cell = INACTIVE;
            } else if (number_of_active_neighbours > 3) {
                *p_current_cell = INACTIVE;
            } else {
                *p_current_cell = ACTIVE;
            }
            // TODO: implement rule with exactly three neighbours.
        }
    }
}

/**
 * This function returns the active cell count for a given x and y coordinate
 * Bounds check should ideally happen before.
 */
short get_number_of_active_neighbours(bool *state, const short x_coordinate, const short y_coordinate) {

    short number_of_active_neighbours = 0;
    bool cell_active;
    bool is_original_cell;
    short test_x;
    short test_y;

    // check neighbourhood of original cell by alternating x and y coordinate
    // by ±1
    for (short dx=-1; dx<=1; dx++) {
        for (short dy=-1; dy<=1; dy++) {
            test_x = x_coordinate + dx;
            test_y = y_coordinate + dy;

            if (cell_inside_bounds(test_x, test_y)) {
                cell_active = cell_is_active(state, test_x, test_y);
                is_original_cell = (dx == 0) && (dy == 0);

                if (cell_active && !(is_original_cell)) {
                    number_of_active_neighbours++;
                }
            }
        }
    }
    return number_of_active_neighbours;
}



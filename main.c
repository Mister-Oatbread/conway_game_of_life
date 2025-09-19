

/**
 * Conway's game of life
 *
 * Author: Mister Oatbread
 */

#include <complex.h>
#include <stdio.h>
#include <time.h>
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

#define REFRESH_RATE 1

#define DAYS2SEC 86400
#define HOURS2SEC 3600
#define MIN2SEC 60

// this index tracks how many lines have been actively written by the program
// and can be used to delete as many
short number_of_lines_printed = 0;

// prototyping
void set_cell_status(bool *state, const short x_coordinate, const short y_coordinate, const bool operation);
void write_cell_state_from_to(bool *next_state, bool *state);
bool cell_is_active(bool *state, const short x_coordinate, const short y_coordinate);
bool cell_inside_bounds(const short x_coordinate, const short y_coordinate);
int calculate_index_with_coordinates(const short x_coordinate, const short y_coordinate);
void update_all_cells(bool *state);
void print_state(bool *state);
short get_active_neighbours(bool *state, const short x_coordinate, const short y_coordinate);
void update_time(long *p_current_time);

int main(void) {

    setlocale(LC_ALL, "");
    // create 150 x 50 block of cells (STATE_SIZE) in total
    printf("Conway's game of life by Mister Oatbread\n\n");
    bool state[STATE_SIZE] = {false};
    bool next_state[STATE_SIZE] = {false};

    // initial condition
    set_cell_status(state, 20, 20, ACTIVE);
    set_cell_status(state, 21, 21, ACTIVE);
    set_cell_status(state, 21, 20, ACTIVE);
    set_cell_status(state, 20, 21, ACTIVE);
    print_state(state);

    // set up time, for now this precise to one second
    long now;
    update_time(&now);
    long last_update = now;

    // main loop
    for (;;) {
        update_time(&now);
        if (now > last_update + REFRESH_RATE) {
            update_time(&last_update);
            // TODO: use cooler time

            update_all_cells(next_state);

            write_cell_state_from_to(next_state, state);
            print_state(state);

        }
    }

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

/**
 * This function can be called to update the cell matrix
 */
void update_all_cells(bool *state) {

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
                number_of_active_neighbours = get_active_neighbours(state, x_coordinate, y_coordinate);
            }

            p_current_cell = state+index;

            // decide if cell should be alive or dead in next step
            // under 2 -> die
            // 2 or 3 -> live
            // over 3 -> die
            // exactly 3 from neighbour -> live
            if (number_of_active_neighbours < 2) {
                *p_current_cell = INACTIVE;
            } else if (number_of_active_neighbours >3) {
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
short get_active_neighbours(bool *state, const short x_coordinate, const short y_coordinate) {

    short number_of_active_neighbours = 0;
    bool cell_active;
    bool not_original_cell;

    // check neighbourhood of original cell by alternating x and y coordinate
    // by ±1
    for (short dx=-1; dx<=1; dx++) {
        for (short dy=-1; dy<=1; dy++) {
            if (cell_inside_bounds(x_coordinate+dx, y_coordinate+dy)) {
                cell_active = cell_is_active(state, x_coordinate + dx, y_coordinate + dy);
                not_original_cell = (dx != 0) && (dy != 0);

                if (cell_active && not_original_cell) {
                    number_of_active_neighbours++;
                }
            }
        }
    }
    return number_of_active_neighbours;
}

/**
 * returns the current time in the time container format
 */
void update_time(long *p_current_time) {
    // get time from time_t
    time_t time_raw;
    time(&time_raw);
    struct tm time;
    time = *(localtime(&time_raw));

    // write to time container
    *p_current_time = time.tm_mday*DAYS2SEC + time.tm_hour*HOURS2SEC + time.tm_min*MIN2SEC + time.tm_sec;
}



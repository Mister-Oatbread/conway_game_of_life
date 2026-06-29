

/**
 * Conway's game of life
 * Main File with all the functionality
 *
 * Author: Mister Oatbread
 */

#include "gol.h"

#include <complex.h>
#include <time.h>
#include <locale.h>
#include <string.h>

short number_of_printed_rows = 0;
bool line_delection_active = true;

int main(void) {

    setlocale(LC_ALL, "");
    // create 150 x 50 block of cells (STATE_SIZE) in total
    printf("Conway's game of life by Mister Oatbread\n");

    int seed = time(NULL);
    srand(seed);
    printf("Seed: %d\n\n", seed);

    bool state[STATE_SIZE] = {false};
    bool next_state[STATE_SIZE] = {false};

    // initial condition
    int index = 0;
    short current_row = 1;
    short current_column = 1;

    bool cell_content;
    bool inside_rows, inside_columns, chosen_alive;

    FILE *initial_condition = fopen("initial_condition.csv", "r");
    short line_size = NUMBER_OF_COLUMNS*SIZE_ENTRY + 2;
    char line[line_size];
    int n;
    char * token;

    initialize_row:
    if (current_row>NUMBER_OF_ROWS) {
        goto end_initialization;
    }
    
    fgets(line, line_size, initial_condition);
    token = strtok(line, ",");

    initialize_column:

    index = calculate_index_with_coordinates(current_column, current_row);
    switch (USE_RANDOM_INITIAL_CONDITION) {
        case true:
            inside_rows = (current_row >= Y_BOUNDS*NUMBER_OF_ROWS) && (current_row <=(1-Y_BOUNDS)*NUMBER_OF_ROWS);
            inside_columns = (current_column >= X_BOUNDS*NUMBER_OF_COLUMNS) && (current_column <=(1-X_BOUNDS)*NUMBER_OF_COLUMNS);
            chosen_alive = (double)rand()/(double)RAND_MAX <= RANDOM_ALIVE_CHANCE;
            cell_content = chosen_alive && inside_rows && inside_columns;
            break;
        case false:
            n = atoi(token);
            cell_content = (n==1);
            break;
    }
    *(state+index) = cell_content;

    if (current_column<=NUMBER_OF_COLUMNS) {
        ++current_column;
        goto initialize_column;
    } else {
        current_column = 1;
        ++current_row;
        goto initialize_row;
    }

    end_initialization:

    fclose(initial_condition);

    print_state(state);

    // set up time, for now this precise to one second
    clock_t current_time, last_time;
    current_time = clock();
    last_time = current_time;

    // main loop
    main_loop_start:
        // check if enoguh time has passed
        current_time = clock();
        goto *(current_time <= last_time + REFRESH_RATE*CLOCKS_PER_SEC/1000 ? &&main_loop_start : &&fallthrough_1);
        fallthrough_1:
        last_time = current_time;

        update_all_cells(state, next_state);
        write_cell_state_from_to(next_state, state);
        print_state(state);

    goto main_loop_start;

    return 0;
}

/**
* This function lets you activate or deactivate a cell at specific x and y coordinates.
* The cell in the top left corner is 1,1; the cell in the bottom right corner is NUMBER_OF_COLUMNS,NUMBER_OF_ROWS
* (may be 100,50; no guarantees).
*/
void set_cell_status(bool* const state, const short x_coordinate, const short y_coordinate, const bool operation) {
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
void write_cell_state_from_to(bool* const next_state, bool* const state) {
    for (int i=0; i<STATE_SIZE; ++i) {
        *(state+i) = *(next_state+i);
    }
}

/**
* This function returns the state of a cell with given x and y coordinates
* If the coordinates are out of bounds, false is returned
*/
bool cell_is_active(const bool* const state, const short x_coordinate, const short y_coordinate) {
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
void print_state(const bool* const state) {
    // TODO: change this to not delete a bunch of stuff at the first iteration
    if (line_delection_active) {
        for (int index=0; index<number_of_printed_rows; index++) {
            printf("\033[A\033[2K");
        }
        number_of_printed_rows = 0;
    }

    for (int index=0; index<STATE_SIZE; ++index) {
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
* The rules are summarized here
* decide if cell should be alive or dead in next step
* alive and under 2 -> dead
* alive and 2 or 3 -> alive
* alive and over 3 -> dead
* dead and exactly 3 -> alive
*/
void update_all_cells(bool* const state, bool* const next_state) {

    short number_of_active_neighbours;
    bool cell_is_alive;
    bool *p_current_cell;
    int index;

    for (short x_coordinate = 1; x_coordinate <= NUMBER_OF_COLUMNS; ++x_coordinate) {
        for (short y_coordinate = 1; y_coordinate <= NUMBER_OF_ROWS; ++y_coordinate) {
            index = calculate_index_with_coordinates(x_coordinate, y_coordinate);
            cell_is_alive = *(state+index);

            // catch cell that is out of bounds, which should not happen
            if (index == -1) {
                printf("updating cells found something out of bounds dingus\n");
                continue;
            } else {
                number_of_active_neighbours = get_number_of_active_neighbours(state, x_coordinate, y_coordinate);
            }

            p_current_cell = next_state+index;

            if (cell_is_alive) {
            // case: cell is alive
                if (number_of_active_neighbours < MIN_2STAY_ALIVE) {
                    *p_current_cell = INACTIVE;
                } else if (number_of_active_neighbours > MAX_2STAY_ALIVE) {
                    *p_current_cell = INACTIVE;
                } else {
                    *p_current_cell = ACTIVE;
                }
            } else {
            // case: cell is dead
                if ((number_of_active_neighbours <= MAX_2B_BORN) && (number_of_active_neighbours >= MIN_2B_BORN)) {
                    *p_current_cell = ACTIVE;
                } else {
                    *p_current_cell = INACTIVE;
                }
            }
        }
    }
}

/**
* This function takes a cell and handles it as if it were an inactive cell
*/
bool handle_dead_cell(bool* const state, const short x_coordinate, const short y_coordinate) {

    bool cell_status = INACTIVE;
    bool cell_is_inside_bounds;

    int number_of_active_neighbours;
    for (short dx=-1; dx<=1; ++dx) {
        for (short dy=-1; dy<=1; ++dy) {

            cell_is_inside_bounds = cell_inside_bounds(x_coordinate, y_coordinate);
            if (!(dx==0 && dy==0) && cell_is_inside_bounds) {
                number_of_active_neighbours = get_number_of_active_neighbours(state, x_coordinate+dx, y_coordinate+dy);
                if (number_of_active_neighbours == 3) {
                    cell_status = ACTIVE;
                    break;
                }
            }
        }
    }
    return cell_status;
}

/**
* This function returns the active cell count for a given x and y coordinate
* Bounds check should ideally happen before.
*/
short get_number_of_active_neighbours(const bool* const state, const short x_coordinate, const short y_coordinate) {

    short number_of_active_neighbours = 0;
    bool cell_active;
    bool is_original_cell;
    short test_x;
    short test_y;

    // check neighbourhood of original cell by alternating x and y coordinate
    // by ±1
    for (short dx=-1; dx<=1; ++dx) {
        for (short dy=-1; dy<=1; ++dy) {
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



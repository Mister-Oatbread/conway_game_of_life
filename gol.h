

#ifndef gol_h
#define gol_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cool macros
#define ACTIVE true
#define INACTIVE false

#define NUMBER_OF_COLUMNS 100
#define NUMBER_OF_ROWS 50
#define STATE_SIZE NUMBER_OF_COLUMNS*NUMBER_OF_ROWS // 100*50 = 5000
#define FULL "●"
#define EMPTY "·"

#define REFRESH_RATE 100 // in milliseconds
#define SIZE_ENTRY 2*sizeof(char)

#define DAYS2SEC 86400
#define HOURS2SEC 3600
#define MIN2SEC 60

// prototyping main
void apply_initial_condition(bool *state);
void set_cell_status(bool *state, const short x_coordinate, const short y_coordinate, const bool operation);
void write_cell_state_from_to(bool *next_state, bool *state);
bool cell_is_active(bool *state, const short x_coordinate, const short y_coordinate);
bool cell_inside_bounds(const short x_coordinate, const short y_coordinate);
int calculate_index_with_coordinates(const short x_coordinate, const short y_coordinate);
void print_state(bool *state);
void update_all_cells(bool *state, bool *next_state);
short get_number_of_active_neighbours(bool *state, const short x_coordinate, const short y_coordinate);

#endif // gol_h



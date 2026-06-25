

#ifndef gol_h
#define gol_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// cool macros
#define ACTIVE true
#define INACTIVE false

#define NUMBER_OF_COLUMNS 118
#define NUMBER_OF_ROWS 58
#define STATE_SIZE NUMBER_OF_COLUMNS*NUMBER_OF_ROWS // 100*50 = 5000
#define FULL "●"
#define EMPTY "·"

#define REFRESH_RATE 100 // in milliseconds
#define SIZE_ENTRY 2*sizeof(char)

#define DAYS2SEC 86400
#define HOURS2SEC 3600
#define MIN2SEC 60

#define MIN_2STAY_ALIVE 2
#define MAX_2STAY_ALIVE 4

#define MIN_2B_BORN 3
#define MAX_2B_BORN 3

#define USE_RANDOM_INITIAL_CONDITION true
#define RANDOM_ALIVE_CHANCE .1

// prototyping main
void apply_initial_condition(bool* const state);
void set_cell_status(bool* const state, const short x_coordinate, const short y_coordinate, const bool operation);
void write_cell_state_from_to(bool* const next_state, bool* const state);
bool cell_is_active(const bool* const state, const short x_coordinate, const short y_coordinate);
bool cell_inside_bounds(const short x_coordinate, const short y_coordinate);
int calculate_index_with_coordinates(const short x_coordinate, const short y_coordinate);
void print_state(const bool* const state);
void update_all_cells(bool* const state, bool* const next_state);
bool handle_dead_cell(bool* const state, const short x_coordinate, const short y_coordinate);
short get_number_of_active_neighbours(const bool* const state, const short x_coordinate, const short y_coordinate);

#endif // gol_h



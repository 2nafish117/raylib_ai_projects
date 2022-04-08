#pragma once

#include <cassert>

#include <common/defines.h>

#include <raylib.h>

struct cell_coord {
	i32 x, y;
};

enum cell_state : u8 {
	STATE_EMPTY,
	STATE_WALL,
	STATE_SOURCE,
	STATE_DEST,
	STATE_VISITED,
	NUM_STATES
};

struct grid_cell {
	i32 distance;
	cell_state state;
};

static Color grid_state_colors[NUM_STATES] = {
	GRAY,	// empty
	BLACK,	// wall
	RED,	// source
	GREEN,	// dest
	BLUE	// visited
};

struct grid {
	grid_cell* cells;

	i32 source_cell_x;
	i32 source_cell_y;
	i32 dest_cell_x;
	i32 dest_cell_y;

	i32 width;
	i32 height;
};

struct game_state {
	grid grid;

	i32 grid_cell_size;
	i32 grid_cell_padding;

	i32 window_width;
	i32 window_height;
};

void grid_create(grid* g, i32 width, i32 height);
void grid_destroy(grid* g);

void game_state_create(game_state* state, i32 grid_cell_count_x, i32 grid_cell_count_y, i32 grid_cell_size, i32 grid_padding = 2);
i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_padding = 2);
void game_state_destroy(game_state* state);

void game_state_handle_resize(game_state* state, i32 width, i32 height);

// set all cell distances
void grid_set_cell_distances(grid* g, i32 distance);

// set all cell states
void grid_set_cell_states(grid* g, cell_state state);

// set cell state
INLINE void grid_set_cell_state(grid* g, cell_coord coord, cell_state value) {
	assert(value < NUM_STATES);
	assert(coord.x >= 0 && coord.y >= 0);
	assert(coord.x < g->width && coord.y < g->height);
	g->cells[coord.x + g->width * coord.y].state = value;
}

// set cell distance
INLINE void grid_set_cell_distance(grid* g, cell_coord coord, i32 distance) {
	assert(coord.x >= 0 && coord.y >= 0);
	assert(coord.x < g->width && coord.y < g->height);
	g->cells[coord.x + g->width * coord.y].distance = distance;
}

// set cell
INLINE void grid_set_cell(grid* g, cell_coord coord, grid_cell c) {
	assert(c.state < NUM_STATES);
	assert(coord.x >= 0 && coord.y >= 0);
	assert(coord.x < g->width && coord.y < g->height);
	g->cells[coord.x + g->width * coord.y] = c;
}

// get cell
INLINE grid_cell grid_get_cell(grid* g, cell_coord coord) {
	assert(coord.x >= 0 && coord.y >= 0);
	assert(coord.x < g->width && coord.y < g->height);
	assert(coord.x + g->width * coord.y < g->width * g->height);
	return g->cells[coord.x + g->width * coord.y];
}

// get cell color
INLINE Color cell_state_get_color(cell_state value) {
	assert(value < NUM_STATES);
	return grid_state_colors[value];
}

// cell coord to window pixels
INLINE cell_coord grid_to_window_coord(game_state* state, grid* g, cell_coord coord) {
	assert(coord.x >= 0 && coord.y >= 0);
	assert(coord.x < g->width && coord.y < g->height);
	return {
		coord.x * state->grid_cell_size + (coord.x + 1) * state->grid_cell_padding,
		coord.y * state->grid_cell_size + (coord.y + 1) * state->grid_cell_padding
	};
}

// window pixels to cell coord
INLINE cell_coord window_to_grid_coord(game_state* state, grid* g, i32 window_x, i32 window_y) {
	window_x = CLAMP(window_x, 0, state->window_width - 1);
	window_y = CLAMP(window_y, 0, state->window_height - 1);
	window_x = window_x * g->width / state->window_width;
	window_y = window_y * g->height / state->window_height;
	return {window_x, window_y};
}
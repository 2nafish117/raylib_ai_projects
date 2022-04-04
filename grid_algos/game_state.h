#pragma once

#include <cassert>

#include <common/defines.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

enum grid_state : u8 {
	STATE_EMPTY,
	STATE_WALL,
	STATE_SOURCE,
	STATE_DEST,
	STATE_VISITED,
	NUM_STATES
};

struct grid_cell {
	i32 distance;
	grid_state state;
};

static sf::Color grid_state_colors[NUM_STATES] = {
	sf::Color(140, 140, 140, 255),	// empty
	sf::Color(128, 255, 255, 255),	// wall
	sf::Color(255,  64, 128, 255),	// source
	sf::Color(128, 255, 128, 255),	// dest
	sf::Color( 64, 255, 255, 255)	// visited
};

struct game_state {
	grid_state* grid;

	grid_state chosen_state;

	i32 grid_cell_size;
	i32 grid_padding;

	i32 grid_cell_count_x;
	i32 grid_cell_count_y;

	i32 window_width;
	i32 window_height;
};

// cell value
INLINE grid_state grid_get_cell_value(game_state* state, i32 x, i32 y) {
	assert(x >= 0 && y >= 0);
	assert(x < state->grid_cell_count_x && y < state->grid_cell_count_y);
	assert(x + state->grid_cell_count_x * y < state->grid_cell_count_x * state->grid_cell_count_y);
	return state->grid[x + state->grid_cell_count_x * y];
}

// cell coord to window pixels
INLINE sf::Vector2f grid_get_cell_pos(game_state* state, i32 x, i32 y) {
	assert(x >= 0 && y >= 0);
	assert(x < state->grid_cell_count_x && y < state->grid_cell_count_y);
	return sf::Vector2f(
		x * state->grid_cell_size + (x + 1) * state->grid_padding,
		y * state->grid_cell_size + (y + 1) * state->grid_padding
	);
}

// cell value to color
INLINE sf::Color grid_get_cell_color(grid_state value) {
	assert(value < NUM_STATES);
	return grid_state_colors[value];
}

// window pixels to cell coord
INLINE sf::Vector2i grid_get_cell_coord(game_state* state, i32 x, i32 y) {
	x = CLAMP(x, 0, state->window_width - 1);
	y = CLAMP(y, 0, state->window_height - 1);
	x = x * state->grid_cell_count_x / state->window_width;
	y = y * state->grid_cell_count_y / state->window_height;
	return sf::Vector2i(x, y);
}

// set cell value
INLINE void grid_set_value(game_state* state, i32 x, i32 y, grid_state value) {
	assert(value < NUM_STATES);
	assert(x >= 0 && y >= 0);
	assert(x < state->grid_cell_count_x && y < state->grid_cell_count_y);
	state->grid[x + state->grid_cell_count_x * y] = value;
}

void game_state_create(game_state* state, i32 grid_cell_count_x, i32 grid_cell_count_y, i32 grid_cell_size, i32 grid_padding = 2);
i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_padding = 2);
void game_state_destroy(game_state* state);


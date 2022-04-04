#pragma once

#include <cassert>

#include <common/defines.h>

// #include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

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

static sf::Color grid_state_colors[NUM_STATES] = {
	sf::Color(140, 140, 140, 255),	// empty
	sf::Color(128, 255, 255, 255),	// wall
	sf::Color(255,  64, 128, 255),	// source
	sf::Color(128, 255, 128, 255),	// dest
	sf::Color( 64, 255, 255, 255)	// visited
};

struct grid {
	grid_cell* cells;

	grid_cell source_cell;
	grid_cell dest_cell;

	i32 width;
	i32 height;
};

struct game_state {
	// grid_state* grid;

	// grid_state chosen_state;

	// i32 grid_cell_count_x;
	// i32 grid_cell_count_y;

	grid grid;

	i32 grid_cell_size;
	i32 grid_cell_padding;

	i32 window_width;
	i32 window_height;

	// sf::RectangleShape grid_cell_shape;
	// sf::RenderWindow window;
};

void grid_create(grid* g, i32 width, i32 height);
void grid_destroy(grid* g);

void game_state_create(game_state* state, i32 grid_cell_count_x, i32 grid_cell_count_y, i32 grid_cell_size, i32 grid_padding = 2);
i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_padding = 2);
void game_state_destroy(game_state* state);

// set cell state
INLINE void grid_set_cell_state(grid* g, i32 x, i32 y, cell_state value) {
	assert(value < NUM_STATES);
	assert(x >= 0 && y >= 0);
	assert(x < g->width && y < g->height);
	g->cells[x + g->width * y].state = value;
}

// set cell distance
INLINE void grid_set_cell_state(grid* g, i32 x, i32 y, i32 distance) {
	assert(x >= 0 && y >= 0);
	assert(x < g->width && y < g->height);
	g->cells[x + g->width * y].distance = distance;
}

// set cell
INLINE void grid_set_cell(grid* g, i32 x, i32 y, grid_cell c) {
	assert(c.state < NUM_STATES);
	assert(x >= 0 && y >= 0);
	assert(x < g->width && y < g->height);
	g->cells[x + g->width * y] = c;
}

// get cell
INLINE grid_cell grid_get_cell(grid* g, i32 x, i32 y) {
	assert(x >= 0 && y >= 0);
	assert(x < g->width && y < g->height);
	assert(x + g->width * y < g->width * g->height);
	return g->cells[x + g->width * y];
}

// get cell color
INLINE sf::Color cell_state_get_color(cell_state value) {
	assert(value < NUM_STATES);
	return grid_state_colors[value];
}

// cell coord to window pixels
INLINE sf::Vector2f grid_to_window_coord(game_state* state, grid* g, i32 grid_x, i32 grid_y) {
	assert(grid_x >= 0 && grid_y >= 0);
	assert(grid_x < g->width && grid_y < g->height);
	return sf::Vector2f(
		grid_x * state->grid_cell_size + (grid_x + 1) * state->grid_cell_padding,
		grid_y * state->grid_cell_size + (grid_y + 1) * state->grid_cell_padding
	);
}

// window pixels to cell coord
INLINE sf::Vector2i window_to_grid_coord(game_state* state, grid* g, i32 window_x, i32 window_y) {
	window_x = CLAMP(window_x, 0, state->window_width - 1);
	window_y = CLAMP(window_y, 0, state->window_height - 1);
	window_x = window_x * g->width / state->window_width;
	window_y = window_y * g->height / state->window_height;
	return sf::Vector2i(window_x, window_y);
}
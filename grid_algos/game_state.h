#pragma once

#include <cassert>
#include <common/defines.h>
#include <raylib.h>

struct grid_coord {
	i32 x, y;
};

enum cell_state : u8 {
	EMPTY,
	BLOCKED
};

struct grid {
	cell_state* states;
	i32* distance;
	b8* visited;

	grid_coord src;
	grid_coord dst;

	i32 width;
	i32 height;
};

struct game_state {
	grid grid;

	i32 grid_cell_size;
	i32 grid_cell_padding;

	i32 window_width;
	i32 window_height;
	i32 text_region_size;
};

void game_state_create(game_state* state, i32 grid_width, i32 grid_height, i32 grid_cell_size, i32 grid_cell_padding = 2);
i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_cell_padding);
void game_state_destroy(game_state* state);

void grid_create(grid* g, i32 width, i32 height);
void grid_destroy(grid* g);

// set all cell distances
void grid_set_cell_distances(grid* g, i32 distance);

// set all cell states
void grid_set_cell_states(grid* g, cell_state state);

// set all visited
void grid_set_all_visited(grid* g, b8 value);

INLINE bool grid_coord_valid(const grid* g, grid_coord coord) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < g->width && coord.y < g->height;
}

// set cell state
INLINE void grid_set_cell_state(grid* g, grid_coord coord, cell_state state) {
	if(grid_coord_valid(g, coord)) {
		g->states[coord.x + g->width * coord.y] = state;
	} else {
		TraceLog(LOG_ERROR, "attempted setting cell state for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// set cell visited
INLINE void grid_set_cell_visited(grid* g, grid_coord coord, b8 visited) {
	if(grid_coord_valid(g, coord)) {
		g->visited[coord.x + g->width * coord.y] = visited;
	} else {
		TraceLog(LOG_ERROR, "attempted setting cell visited for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// set cell distance
INLINE void grid_set_cell_distance(grid* g, grid_coord coord, i32 distance) {
	if(grid_coord_valid(g, coord)) {
		g->distance[coord.x + g->width * coord.y] = distance;

	} else {
		TraceLog(LOG_ERROR, "attempted setting cell distance for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// get cell state
INLINE cell_state grid_get_cell_state(grid* g, grid_coord coord) {
	assert(grid_coord_valid(g, coord));
	return g->states[coord.x + g->width * coord.y];
}

// get cell visited
INLINE b8 grid_get_cell_visited(grid* g, grid_coord coord) {
	assert(grid_coord_valid(g, coord));
	return g->visited[coord.x + g->width * coord.y];
}

// get cell distance
INLINE i32 grid_get_cell_distance(grid* g, grid_coord coord) {
	assert(grid_coord_valid(g, coord));
	return g->distance[coord.x + g->width * coord.y];
}

// get cell color
Color grid_get_cell_color(grid* g, grid_coord coord);

// grid coord to window pixels
INLINE Vector2 grid_to_window_coord(game_state* state, grid* g, grid_coord coord) {
	assert(coord.x >= 0 && coord.y >= 0);
	assert(coord.x < g->width && coord.y < g->height);
	return {
		(f32) (coord.x * state->grid_cell_size + (coord.x + 1) * state->grid_cell_padding),
		(f32) (coord.y * state->grid_cell_size + (coord.y + 1) * state->grid_cell_padding)
	};
}

// window pixels to grid coord
INLINE grid_coord window_to_grid_coord(game_state* state, grid* g, Vector2 win_coord) {
	win_coord.x = CLAMP(win_coord.x, 0, state->window_width - 1);
	win_coord.y = CLAMP(win_coord.y, 0, state->window_height - 1);
	win_coord.x = win_coord.x * g->width / (state->window_width - state->text_region_size);
	win_coord.y = win_coord.y * g->height / state->window_height;
	return {(i32) win_coord.x, (i32) win_coord.y};
}


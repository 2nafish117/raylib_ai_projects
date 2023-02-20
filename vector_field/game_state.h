#pragma once

#include <cassert>
#include <common.h>

#include <cstdio>
#include <raylib.h>

#include "particle_system.h"

struct grid_coord {
	i32 x, y;
};

enum cell_state : u8 {
	EMPTY,
	BLOCKED
};

struct grid {
	cell_state* states;
	b8* visited;
	i32* distance;
	Vector2* gradient;

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

	b8 show_distances;
	b8 show_gradients;

	// font at 0 index is useless
	Font font[65];

	particle_system particles;
};

void game_state_create(game_state* state, i32 grid_width, i32 grid_height, i32 grid_cell_size, i32 grid_cell_padding = 2);
i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_cell_padding);
void game_state_destroy(game_state* state);

void grid_create(grid* g, i32 width, i32 height);
void grid_destroy(grid* g);
void grid_calculate_distances(const grid* const g);
void grid_calculate_gradients(const grid* const g);

// set all cell distances
void grid_set_cell_distances(const grid* const g, i32 distance);

// set all cell states
void grid_set_cell_states(const grid* const g, cell_state state);

// set all visited
void grid_set_all_visited(const grid* const g, b8 value);

inline bool grid_coord_valid(const grid* const g, grid_coord coord) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < g->width && coord.y < g->height;
}

// set cell state
inline void grid_set_cell_state(const grid* const g, grid_coord coord, cell_state state) {
	if(grid_coord_valid(g, coord)) {
		g->states[coord.x + g->width * coord.y] = state;
	} else {
		TraceLog(LOG_ERROR, "attempted setting cell state for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// set cell visited
inline void grid_set_cell_visited(const grid* const g, grid_coord coord, b8 visited) {
	if(grid_coord_valid(g, coord)) {
		g->visited[coord.x + g->width * coord.y] = visited;
	} else {
		TraceLog(LOG_ERROR, "attempted setting cell visited for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// set cell distance
inline void grid_set_cell_distance(const grid* const g, grid_coord coord, i32 distance) {
	if(grid_coord_valid(g, coord)) {
		g->distance[coord.x + g->width * coord.y] = distance;

	} else {
		TraceLog(LOG_ERROR, "attempted setting cell distance for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// set cell gradient
inline void grid_set_cell_gradient(const grid* const g, grid_coord coord, Vector2 gradient) {
	if(grid_coord_valid(g, coord)) {
		g->gradient[coord.x + g->width * coord.y] = gradient;
	} else {
		TraceLog(LOG_ERROR, "attempted setting cell gradient for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
}

// get cell state
inline cell_state grid_get_cell_state(const grid* const g, grid_coord coord) {
	if(grid_coord_valid(g, coord)) {
		return g->states[coord.x + g->width * coord.y];
	} else {
		TraceLog(LOG_ERROR, "attempted getting cell state for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
	return cell_state::EMPTY;
}

// get cell visited
inline b8 grid_get_cell_visited(const grid* const g, grid_coord coord) {
	if(grid_coord_valid(g, coord)) {
		return g->visited[coord.x + g->width * coord.y];
	} else {
		TraceLog(LOG_ERROR, "attempted getting cell visited for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
	return false;
}

// get cell distance
inline i32 grid_get_cell_distance(const grid* const g, grid_coord coord) {
	if(grid_coord_valid(g, coord)) {
		return g->distance[coord.x + g->width * coord.y];
	} else {
		TraceLog(LOG_ERROR, "attempted getting cell distance for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
	return 0;
}

// get cell gradient
inline Vector2 grid_get_cell_gradient(const grid* const g, grid_coord coord) {
	if(grid_coord_valid(g, coord)) {
		return g->gradient[coord.x + g->width * coord.y];
	} else {
		TraceLog(LOG_ERROR, "attempted getting cell gradient for out of bound grid_coord: {%d, %d}", coord.x, coord.y);
	}
	return {};
}

// get cell color
Color grid_get_cell_color(const grid* const g, grid_coord coord);

// grid coord to window pixels of cells top left corner
inline Vector2 grid_to_window_coord(const game_state* const state, const grid* const g, grid_coord coord) {
	// assert(coord.x >= 0 && coord.y >= 0);
	// assert(coord.x < g->width && coord.y < g->height);
	return {
		(f32) (coord.x * (state->grid_cell_size + state->grid_cell_padding)),
		(f32) (coord.y * (state->grid_cell_size + state->grid_cell_padding))
	};
}

// window pixels to grid coord
inline grid_coord window_to_grid_coord(const game_state* const state, const grid* const g, Vector2 win_coord) {
	win_coord.x = win_coord.x / (state->grid_cell_size + state->grid_cell_padding);
	win_coord.y = win_coord.y / (state->grid_cell_size + state->grid_cell_padding);

	// win_coord.x = CLAMP(win_coord.x, 0, g->width - 1);
	// win_coord.y = CLAMP(win_coord.y, 0, g->height - 1);

	return {(i32) win_coord.x, (i32) win_coord.y};
}
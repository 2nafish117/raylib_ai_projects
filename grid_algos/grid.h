#pragma once

#include <cassert>
#include <raylib.h>
#include <common/defines.h>

#include "game_state.h"

struct grid_coord {
	i32 x, y;
};

enum {
	CELL_BLOCKED_BIT = 0x1,
	CELL_VISITED_BIT = 0x2,
	CELL_SRC_BIT     = 0x4,
	CELL_DST_BIT 	 = 0x8
};

typedef u8 cell_flags;

struct grid_cell {
	i32 distance;
	cell_flags flags;
};

struct grid {
	grid_cell* cells;

	grid_coord src;
	grid_coord dst;

	i32 width;
	i32 height;
};

void grid_create(grid* g, i32 width, i32 height);
void grid_destroy(grid* g);

// set all cell distances
void grid_set_all_cell_distances(grid* g, i32 distance);

// set all cell flags
void grid_set_all_cell_flags(grid* g, cell_flags flags);

// reset visited
void grid_reset_visited(grid* g);

INLINE bool grid_coord_valid(const grid* g, grid_coord coord) {
	return coord.x >= 0 && coord.y >= 0 && coord.x < g->width && coord.y < g->height;
}

// set src
INLINE void grid_set_src(grid* g, grid_coord src) {
	if(grid_coord_valid(g, g->src) && grid_get_cell(g, g->src).flags & CELL_SRC_BIT) {
		grid_set_cell_flags(g, g->src, 0);
	}
	grid_set_cell_flags(g, src, CELL_SRC_BIT);
}

// set dst
INLINE void grid_set_dst(grid* g, grid_coord dst) {
	if(grid_coord_valid(g, g->dst) && grid_get_cell(g, g->dst).flags & CELL_DST_BIT) {
		grid_set_cell_flags(g, g->dst, 0);
	}
	grid_set_cell_flags(g, dst, CELL_DST_BIT);
}

// set cell state
INLINE void grid_set_cell_flags(grid* g, grid_coord coord, cell_flags flags) {
	assert(grid_coord_valid(g, coord));
	g->cells[coord.x + g->width * coord.y].flags = flags;
}

// set cell distance
INLINE void grid_set_cell_distance(grid* g, grid_coord coord, i32 distance) {
	assert(grid_coord_valid(g, coord));
	g->cells[coord.x + g->width * coord.y].distance = distance;
}

// set cell
INLINE void grid_set_cell(grid* g, grid_coord coord, grid_cell c) {
	assert(grid_coord_valid(g, coord));
	g->cells[coord.x + g->width * coord.y] = c;
}

// get cell
INLINE grid_cell grid_get_cell(const grid* g, grid_coord coord) {
	assert(grid_coord_valid(g, coord));
	return g->cells[coord.x + g->width * coord.y];
}

// get cell color
INLINE Color cell_state_get_color(cell_flags flags) {
	if(flags & CELL_BLOCKED_BIT) {
		return {50, 50, 50, 255};
	}
	if(flags & CELL_VISITED_BIT) {
		return {50, 50, 50, 255};
	}
	if(flags & CELL_SRC_BIT) {
		return {200, 50, 50, 255};
	}
	if(flags & CELL_DST_BIT) {
		return {50, 200, 50, 255};
	}
	return {200, 200, 200, 255};
}

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
	win_coord.x = win_coord.x * g->width / state->window_width;
	win_coord.x = win_coord.y * g->height / state->window_height;
	return {(i32) win_coord.x, (i32) win_coord.y};
}
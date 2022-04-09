#include "game_state.h"

#include <cstdio>

void grid_create(grid* g, i32 width, i32 height) {
	assert(width > 0 && height > 0);
	
	g->width = width;
	g->height = height;
	
	g->cells = new grid_cell[width * height];
	memset(g->cells, 0, sizeof(grid_cell) * width * height);
}

void grid_destroy(grid* g) {
	delete[] g->cells;
	*g = {0};
}

void grid_reset_visited(grid* g) {
	for(i32 i = 0; i < g->width * g->height; ++i) {
		g->cells[i].flags = g->cells[i].flags & ~CELL_VISITED_BIT;
	}
}

// set all cell distances
void grid_set_all_cell_distances(grid* g, i32 distance) {
	for(i32 i = 0;i < g->height * g->width; ++i) {
		g->cells[i].distance = distance;
	}
}

// set all cell flags
void grid_set_all_cell_flags(grid* g, cell_flags flags) {
	for(i32 i = 0;i < g->height * g->width; ++i) {
		g->cells[i].flags = flags;
	}
}

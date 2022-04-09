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

void game_state_create(
		game_state* state,
		i32 grid_width,
		i32 grid_height,
		i32 grid_cell_size, i32 grid_cell_padding) {
	
	state->grid_cell_padding = grid_cell_padding;
	state->grid_cell_size = grid_cell_size;

	grid_create(&state->grid, grid_width, grid_height);

	// right text area = 200 + actual space for cells
	i32 text_region_size = 200;
	i32 window_width = text_region_size + grid_cell_size * grid_width + grid_cell_padding * (grid_width + 1);
	i32 window_height = grid_cell_size * grid_height + grid_cell_padding * (grid_height + 1);

	state->text_region_size = text_region_size;
	state->window_width = window_width;
	state->window_height = window_height;
}

i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_cell_padding) {
	FILE* handle = NULL;
	errno_t err = fopen_s(&handle, path, "r");
	if(err != 0) {
		printf("file (%s) doesn't exist\n", path);
		return -1; 
	}

	i32 grid_width, grid_height;
	fscanf(handle, "%d %d", &grid_width, &grid_height);
	printf("%d %d\n", grid_width, grid_height);

	game_state_create(state, grid_width, grid_height, grid_cell_size, grid_cell_padding);

	grid* grid = &state->grid;

	for(i32 y = 0; y < grid_height; ++y) {
		for(i32 x = 0; x < grid_width; ++x) {
			char c;
			do {
				// ignore whitespace chars
				fscanf(handle, "%c", &c);
			} while(c == ' ' || c == '\t' || c == '\n');

			printf("%c", c);
			switch(c) {
				case '.': {
					// empty
					grid_set_cell_flags(grid, {x, y}, 0);
				} break;
				case '#': {
					// wall
					grid_set_cell_flags(grid, {x, y} , CELL_BLOCKED_BIT);
				} break;
				case 's':
				case 'S': {
					// source
					grid_set_src(grid, {x, y});
					// grid_set_cell_flags(grid, {x, y} , CELL_SRC_BIT);
					// grid->src = {x, y};
				} break;
				case 'd':
				case 'D': {
					// destination
					grid_set_dst(grid, {x, y});
					// grid_set_cell_flags(grid, {x, y} , CELL_DST_BIT);
					// grid->dst = {x, y};
				} break;
				default: {
					printf("\ninvalid grid charecter %c in file %s at x: %d y: %d\n", c, path, x, y);
				} break;
			}
		}
		printf("\n");
	}

	fclose(handle);
	return 0;
}

void game_state_destroy(game_state* state) {
	grid_destroy(&state->grid);
	*state = {0};
}
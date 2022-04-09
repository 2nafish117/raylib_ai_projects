#include "game_state.h"

#include <cstdio>

void game_state_create(game_state* state, i32 grid_width, i32 grid_height, i32 grid_cell_size, i32 grid_cell_padding) {
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
					grid_set_cell_state(grid, {x, y}, cell_state::EMPTY);
				} break;
				case '#': {
					// wall
					grid_set_cell_state(grid, {x, y}, cell_state::BLOCKED);
				} break;
				case 's':
				case 'S': {
					// source
					grid->src = {x, y};
				} break;
				case 'd':
				case 'D': {
					// destination
					grid->dst = {x, y};
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

void grid_create(grid* g, i32 width, i32 height) {
	assert(width > 0 && height > 0);
	
	g->width = width;
	g->height = height;
	
	// u64 total_mem_requirement = width * height * (sizeof(cell_state) + sizeof(b8) + sizeof(i32));
	// void* mem = malloc(total_mem_requirement);
	// memset(mem, 0, total_mem_requirement);

	// g->states = (cell_state*) ((u64) mem + 0);
	// g->visited = (b8*) ((u64) mem + width * height * sizeof(cell_state));
	// g->distance = (i32*) ((u64) mem + width * height * sizeof(cell_state) + sizeof(b8));

	g->states = (cell_state*) malloc(width * height * sizeof(cell_state));
	memset(g->states, 0, width * height * sizeof(cell_state));

	g->visited = (b8*) malloc(width * height * sizeof(b8));
	memset(g->visited, 0, width * height * sizeof(b8));

	g->distance = (i32*) malloc(width * height * sizeof(i32));
	memset(g->distance, 0, width * height * sizeof(i32));
}

void grid_destroy(grid* g) {
	// free(g->states);
	*g = {0};
}

void grid_set_all_visited(grid* g, b8 value) {
	memset(g->visited, 0, g->width * g->height * sizeof(b8));
}

// set all cell distances
void grid_set_cell_distances(grid* g, i32 distance) {
	for(i32 i = 0;i < g->height * g->width; ++i) {
		g->distance[i] = distance;
	}
}

// set all cell states
void grid_set_cell_states(grid* g, cell_state state) {
	for(i32 i = 0;i < g->height * g->width; ++i) {
		g->states[i] = state;
	}
}

// get cell color
Color grid_get_cell_color(grid* g, grid_coord coord) {
	if(g->src.x == coord.x && g->src.y == coord.y) {
		return {200, 50, 50, 255};
	}
	if(g->dst.x == coord.x && g->dst.y == coord.y) {
		return {50, 200, 50, 255};
	}
	if(grid_get_cell_state(g, coord) == cell_state::EMPTY) {
		return {100, 100, 100, 255};
	}
	if(grid_get_cell_state(g, coord) == cell_state::BLOCKED) {
		return {50, 50, 50, 255};
	}
	// if(grid_get_cell_visited(g, coord)) {
	// 	return {50, 50, 50, 255};
	// }
	return {200, 200, 200, 255};
}

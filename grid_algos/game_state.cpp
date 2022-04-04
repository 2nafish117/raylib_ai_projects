#include "game_state.h"

#include <cstdio>

void grid_create(grid* g, i32 width, i32 height) {
	assert(width > 0 && height > 0);
	
	g->width = width;
	g->height = height;
	
	g->cells = new grid_cell[width * height];
	memset(g->cells, cell_state::STATE_EMPTY, sizeof(grid_cell) * width * height);
}

void grid_destroy(grid* g) {
	delete[] g->cells;
	*g = {0};
}

void game_state_create(
		game_state* state,
		i32 grid_width,
		i32 grid_height,
		i32 grid_cell_size, i32 grid_cell_padding) {
	
	state->grid_cell_padding = grid_cell_padding;
	state->grid_cell_size = grid_cell_size;

	grid_create(&state->grid, grid_width, grid_height);

	i32 window_width = grid_cell_size * grid_width + grid_cell_padding * (grid_width + 1);
	i32 window_height = grid_cell_size * grid_height + grid_cell_padding * (grid_height + 1);

	state->window_width = window_width;
	state->window_height = window_height;

	// state->grid_cell_shape = sf::RectangleShape(sf::Vector2f((f32) grid_cell_size, (f32) grid_cell_size));
	// state->window = sf::RenderWindow(sf::VideoMode(window_width, window_height), "grid algos");
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
					grid_set_cell_state(grid, x, y, cell_state::STATE_EMPTY);
				} break;
				case '#': {
					// wall
					grid_set_cell_state(grid, x, y , cell_state::STATE_WALL);
				} break;
				case 's':
				case 'S': {
					// source
					grid_set_cell_state(grid, x, y , cell_state::STATE_SOURCE);
				} break;
				case 'd':
				case 'D': {
					// destination
					grid_set_cell_state(grid, x, y , cell_state::STATE_DEST);
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
}
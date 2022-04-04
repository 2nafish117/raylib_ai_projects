#include "game_state.h"

#include <cstdio>

void game_state_create(
		game_state* state,
		i32 grid_cell_count_x,
		i32 grid_cell_count_y,
		i32 grid_cell_size, i32 grid_padding) {

	assert(grid_cell_count_x > 0 && grid_cell_count_y > 0);
	assert(grid_cell_size > 0 && grid_padding > 0);
	
	state->grid_cell_count_x = grid_cell_count_x;
	state->grid_cell_count_y = grid_cell_count_y;
	
	state->grid_padding = grid_padding;

	state->grid_cell_size = grid_cell_size;
	
	state->grid = new grid_state[grid_cell_count_x * grid_cell_count_y];
	memset(state->grid, 0, sizeof(grid_state) * grid_cell_count_x * grid_cell_count_y);

	state->window_width = grid_cell_size * grid_cell_count_x + grid_padding * (grid_cell_count_x + 1);
	state->window_height = grid_cell_size * grid_cell_count_y + grid_padding * (grid_cell_count_y + 1);
}

i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_padding) {
	FILE* handle = NULL;
	errno_t err = fopen_s(&handle, path, "r");
	if(err != 0) {
		printf("file (%s) doesnt exist\n", path);
		return -1; 
	}

	fscanf(handle, "%d %d", &state->grid_cell_count_x, &state->grid_cell_count_y);

	printf("%d %d\n", state->grid_cell_count_x, state->grid_cell_count_y);

	game_state_create(state, state->grid_cell_count_x, state->grid_cell_count_y, grid_cell_size, grid_padding);

	for(i32 y = 0; y < state->grid_cell_count_y; ++y) {
		for(i32 x = 0; x < state->grid_cell_count_x; ++x) {
			char c;
			do {
				// ignore whitespace chars
				fscanf(handle, "%c", &c);
			} while(c == ' ' || c == '\t' || c == '\n');

			printf("%c", c);
			switch(c) {
				case '.': {
					// empty
					grid_set_value(state, x, y , grid_state::STATE_EMPTY);
				} break;
				case '#': {
					// wall
					grid_set_value(state, x, y , grid_state::STATE_WALL);
				} break;
				case 's':
				case 'S': {
					// source
					grid_set_value(state, x, y , grid_state::STATE_SOURCE);
				} break;
				case 'd':
				case 'D': {
					// destination
					grid_set_value(state, x, y , grid_state::STATE_DEST);
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
	delete[] state->grid;
	*state = {0};
}
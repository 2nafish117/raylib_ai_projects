#include "game_state.h"

#include <queue>
#include <stack>

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
	
	state->show_distances = true;
	state->show_gradients = true;

	particle_system_create(&state->particles, 100);
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
	
	for(i32 font_size = 1; font_size < 65; ++font_size) {
		UnloadFont(state->font[font_size]);
	}

	particle_system_destroy(&state->particles);
	*state = {0};
}

void grid_create(grid* g, i32 width, i32 height) {
	assert(width > 0 && height > 0);
	
	g->width = width;
	g->height = height;
	
	u64 total_mem_requirement = width * height * (sizeof(cell_state) + sizeof(b8) + sizeof(i32) + sizeof(Vector2));
	void* mem = malloc(total_mem_requirement);
	memset(mem, 0, total_mem_requirement);

	g->states   = (cell_state*)     mem;
	g->visited  = (b8*)      ((u64) mem + width * height * (sizeof(cell_state)));
	g->distance = (i32*)     ((u64) mem + width * height * (sizeof(cell_state) + sizeof(b8)));
	g->gradient = (Vector2*) ((u64) mem + width * height * (sizeof(cell_state) + sizeof(b8) + sizeof(i32)));
}

void grid_destroy(grid* g) {
	free(g->states);
	*g = {0};
}

void grid_set_all_visited(const grid* const g, b8 value) {
	memset(g->visited, 0, g->width * g->height * sizeof(b8));
}

// set all cell distances
void grid_set_cell_distances(const grid* const g, i32 distance) {
	for(i32 i = 0;i < g->height * g->width; ++i) {
		g->distance[i] = distance;
	}
}

// set all cell states
void grid_fill_cell_states(const grid* const g, cell_state state) {
	for(i32 i = 0;i < g->height * g->width; ++i) {
		g->states[i] = state;
	}
}

// get cell color
Color grid_get_cell_color(const grid* const g, grid_coord coord) {
	// if(g->src.x == coord.x && g->src.y == coord.y) {
	// 	return {200, 50, 50, 255};
	// }
	if(g->dst.x == coord.x && g->dst.y == coord.y) {
		return {50, 200, 50, 255};
	}
	// if(grid_get_cell_visited(g, coord)) {
	// 	return {150, 150, 50, 255};
	// }
	if(grid_get_cell_state(g, coord) == cell_state::EMPTY) {
		return {100, 100, 100, 255};
	}
	if(grid_get_cell_state(g, coord) == cell_state::BLOCKED) {
		return {50, 50, 50, 255};
	}

	return {200, 200, 200, 255};
}


void grid_calculate_distances(const grid* const g) {
	std::queue<grid_coord> queue;

	//set all distance as INT_MAX
	grid_set_cell_distances(g, INT32_MAX);
	
	//set distance to source as 0
	grid_set_cell_distance(g, g->dst, 0);

	// push source into queue
	queue.push(g->dst);

	while(!queue.empty()) {
		grid_coord coord = queue.front();
		queue.pop();

		i32 curr_distance = grid_get_cell_distance(g, coord);

		grid_coord neighbour_coords[4] = {
			// vertical horizontal
			{coord.x, coord.y - 1},
			{coord.x, coord.y + 1},
			{coord.x - 1, coord.y},
			{coord.x + 1, coord.y}
		};

		// for each neighbour check and push on queue
		for(i32 i = 0;i < 4; ++i) {
			grid_coord n = neighbour_coords[i];
			if(!grid_coord_valid(g, n)) {
				continue;
			}
			i32 n_distance = grid_get_cell_distance(g, n);
			cell_state n_state = grid_get_cell_state(g, n);

			if(n_distance == INT32_MAX && n_state != cell_state::BLOCKED) {
				grid_set_cell_distance(g, n, 1 + curr_distance);
				queue.push(n);
			}
		}
	}
}

void grid_calculate_gradients(const grid* const g) {
	grid_calculate_distances(g);

	for(i32 x = 0; x < g->width; ++x) {
		for(i32 y = 0; y < g->width; ++y) {
			i32 dist = grid_get_cell_distance(g, {x, y});
			Vector2 gradient = {0};

			if(grid_get_cell_state(g, {x, y}) == cell_state::BLOCKED) {
				continue;
			}

			if(x - 1 >= 0 && grid_get_cell_state(g, {x - 1, y}) != cell_state::BLOCKED) {
				gradient.x += grid_get_cell_distance(g, {x - 1, y});
			} else {
				gradient.x += dist + 1;
			}
			if(x + 1 < g->width && grid_get_cell_state(g, {x + 1, y}) != cell_state::BLOCKED) {
				gradient.x -= grid_get_cell_distance(g, {x + 1, y});
			} else {
				gradient.x -= dist + 1;
			}

			if(y - 1 >= 0 && grid_get_cell_state(g, {x, y - 1}) != cell_state::BLOCKED) {
				gradient.y += grid_get_cell_distance(g, {x, y - 1});
			} else {
				gradient.y += dist + 1;
			}
			if(y + 1 < g->height && grid_get_cell_state(g, {x, y + 1}) != cell_state::BLOCKED) {
				gradient.y -= grid_get_cell_distance(g, {x, y + 1});
			} else {
				gradient.y -= dist + 1;
			}

			// if(grid_get_cell_state(g, {x, y}) == cell_state::BLOCKED) {
			// 	gradient.x = -gradient.x;
			// 	gradient.y = -gradient.y;
			// }

			f32 length = sqrtf(gradient.x * gradient.x + gradient.y * gradient.y);
			if(length == 0.0) {
				length = 1.0;
			}
			gradient = {gradient.x / length, gradient.y / length};

			grid_set_cell_gradient(g, {x, y}, gradient);
		}
	}
}
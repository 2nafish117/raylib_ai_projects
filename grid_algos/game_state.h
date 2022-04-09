#pragma once

#include <cassert>
#include <common/defines.h>
#include "grid.h"

struct game_state {
	grid grid;

	i32 grid_cell_size;
	i32 grid_cell_padding;

	i32 window_width;
	i32 window_height;
	i32 text_region_size;
};

void game_state_create(game_state* state, i32 grid_cell_count_x, i32 grid_cell_count_y, i32 grid_cell_size, i32 grid_padding = 2);
i32 game_state_create_from_file(game_state* state, const char* path, i32 grid_cell_size, i32 grid_padding = 2);
void game_state_destroy(game_state* state);

void game_state_handle_resize(game_state* state, i32 width, i32 height);

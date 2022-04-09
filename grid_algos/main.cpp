#include <iostream>

#include <raylib.h>

#include "game_state.h"
#include "grid_solvers.h"

int main()
{
	game_state state;
	memset(&state, 0, sizeof(game_state));
	
	// const char* cwd = std::filesystem::current_path().generic_string().c_str();
	// std::cout << "running from: " << cwd << "\n";

	i32 err = game_state_create_from_file(&state, "assets/grids/grid2.txt", 32, 2);
	if(err != 0) {
		return err;
	}

	InitWindow(state.window_width, state.window_height, "grid algos");
	SetTargetFPS(60);

	TraceLog(LOG_DEBUG, "hello from raylib logger %i %s %f", 32, "stringer", 54.56);

	Font font = LoadFont("assets/bebas-neue/BebasNeue-Regular.ttf");

	while (!WindowShouldClose()) {

		// Input
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			// add wall	
			i32 mouse_x = GetMouseX();
			i32 mouse_y = GetMouseY();
			grid_coord coord = window_to_grid_coord(&state, &state.grid, mouse_x, mouse_y);
			grid_set_cell_state(&state.grid, coord, cell_state::STATE_WALL);
		}
		if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			// remove wall	
			i32 mouse_x = GetMouseX();
			i32 mouse_y = GetMouseY();
			grid_coord coord = window_to_grid_coord(&state, &state.grid, mouse_x, mouse_y);
			grid_set_cell_state(&state.grid, coord, cell_state::STATE_EMPTY);
		}
		if(IsKeyPressed(KEY_A)) {
			// solve graph
			grid_traverse_bfs(&state.grid, state.grid.src, true);
		}


		// Update

		// Drawing
		
		BeginDrawing();

			ClearBackground(RAYWHITE);

			for(i32 y = 0; y < state.grid.height; ++y) {
				for(i32 x = 0; x < state.grid.width; ++x) {
					grid_cell cell = grid_get_cell(&state.grid, {x, y});
					grid_coord coord = grid_to_window_coord(&state, &state.grid, {x, y});
					Color cell_color = cell_state_get_color(cell.state);

					DrawRectangle(coord.x, coord.y, state.grid_cell_size, state.grid_cell_size, cell_color);
					char dist_str_buf[16] = {0};
					if(cell.distance == INT32_MAX) {
						strcpy_s(dist_str_buf, "X");
					} else {
						_itoa_s(cell.distance, dist_str_buf, 10);
					}
					DrawTextEx(font, dist_str_buf, {(f32)coord.x, (f32)coord.y}, 24, 2, GREEN);
				}	
			}

		EndDrawing();
	}

	game_state_destroy(&state);
	CloseWindow();
	return 0;
}
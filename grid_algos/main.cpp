#include <iostream>

#include <raylib.h>

#include "game_state.h"

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

	Font font = LoadFont("assets/bebas-neue/BebasNeue-Regular.ttf");

	while (!WindowShouldClose()) {

		// Input
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			// add wall	
			i32 mouse_x = GetMouseX();
			i32 mouse_y = GetMouseY();
			cell_coord coord = window_to_grid_coord(&state, &state.grid, mouse_x, mouse_y);
			grid_set_cell_state(&state.grid, coord, cell_state::STATE_WALL);
		}
		if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			// remove wall	
			i32 mouse_x = GetMouseX();
			i32 mouse_y = GetMouseY();
			cell_coord coord = window_to_grid_coord(&state, &state.grid, mouse_x, mouse_y);
			grid_set_cell_state(&state.grid, coord, cell_state::STATE_EMPTY);
		}


		// Update

		// Drawing
		
		BeginDrawing();

			ClearBackground(RAYWHITE);

			for(i32 y = 0; y < state.grid.height; ++y) {
				for(i32 x = 0; x < state.grid.width; ++x) {
					grid_cell cell = grid_get_cell(&state.grid, {x, y});
					cell_coord coord = grid_to_window_coord(&state, &state.grid, {x, y});
					Color cell_color = cell_state_get_color(cell.state);

					DrawRectangle(coord.x, coord.y, state.grid_cell_size, state.grid_cell_size, cell_color);
				}	
			}

			DrawTextEx(font, "new font", {50, 50}, 50, 50, DARKBROWN);

		EndDrawing();
	}

	game_state_destroy(&state);
	CloseWindow();
	return 0;
}
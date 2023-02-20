#include <iostream>
#include <raylib.h>

#include "game_state.h"
#include "particle_system.h"

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
	SetTargetFPS(144);

	
	for(i32 font_size = 1; font_size < 65; ++font_size) {
		state.font[font_size] = LoadFontEx("assets/bebas-neue/BebasNeue-Regular.ttf", font_size, NULL, 0);
	}

	grid_calculate_gradients(&state.grid);
	particle_system_randomize(&state.particles, {0, 0}, {(f32) state.window_width - state.text_region_size, (f32) state.window_height});

	while (!WindowShouldClose()) {
		// Input
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			// add wall	
			i32 mouse_x = GetMouseX();
			i32 mouse_y = GetMouseY();
			grid_coord coord = window_to_grid_coord(&state, &state.grid, {(f32) mouse_x, (f32) mouse_y});
			if(grid_get_cell_state(&state.grid, coord) != cell_state::BLOCKED) {
				state.grid.dst = coord;
				grid_calculate_gradients(&state.grid);
			}
		}
		// if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		// 	// test transformation
		// 	i32 mouse_x = GetMouseX();
		// 	i32 mouse_y = GetMouseY();
		// 	grid_coord coord = window_to_grid_coord(&state, &state.grid, {(f32) mouse_x, (f32) mouse_y});
		// 	TraceLog(LOG_INFO, "mouse: %d %d :: grid: %d %d", mouse_x, mouse_y, coord.x, coord.y);
		// }

		// Update
		{
			f32 dt = GetFrameTime();
			particle_system_update(&state.particles, &state, &state.grid, 1.0, dt);
		}

		// Drawing
		BeginDrawing(); {
			ClearBackground(RAYWHITE);

			// draw grid
			for(i32 y = 0; y < state.grid.height; ++y) {
				for(i32 x = 0; x < state.grid.width; ++x) {
					Vector2 coord = grid_to_window_coord(&state, &state.grid, {x, y});
					Color cell_color = grid_get_cell_color(&state.grid, {x, y});
					i32 cell_distance = grid_get_cell_distance(&state.grid, {x, y});

					// draw grid cell
					DrawRectangle((i32) coord.x, (i32) coord.y, state.grid_cell_size, state.grid_cell_size, cell_color);

					char dist_str_buf[16] = {0};
					if(cell_distance == INT32_MAX) {
						// strcpy_s(dist_str_buf, "X");
					} else {
						_itoa_s(cell_distance, dist_str_buf, 10);
					}
					// draw distances
					DrawTextEx(state.font[18], dist_str_buf, {(f32)coord.x, (f32)coord.y}, 18, 0, RAYWHITE);
					Vector2 gradient = grid_get_cell_gradient(&state.grid, {x, y});

					// if(cell_distance != INT32_MAX) 
					{
						// draw gradients
						i32 start_x = (i32) (coord.x + state.grid_cell_size * 0.5f);
						i32 start_y = (i32) (coord.y + state.grid_cell_size * 0.5f);
						DrawRectangle(start_x - 2, start_y - 2, 4, 4, RED);
						DrawLine(start_x, start_y, start_x + gradient.x * 12, start_y + gradient.y * 12, ORANGE);
					}
				}	
			}
			
			// draw particles
			{
				for(i32 i = 0; i < state.particles.particle_count; ++i) {
					DrawRectangle((i32) state.particles.particles[i].position.x - 4, (i32) state.particles.particles[i].position.y - 4, 8, 8, YELLOW);
				}
			}

			// draw info text
			{
				f32 info_text_x = state.window_width - state.text_region_size + 20;
				f32 info_text_y = 20;
				DrawTextEx(state.font[28], "   INFO", {info_text_x, info_text_y}, 28, 0, BLACK);
				DrawTextEx(state.font[28], "RMB: move destination", {info_text_x, info_text_y + 32}, 28, 0, DARKGRAY);
				DrawTextEx(state.font[28], "Q: toggle distances", {info_text_x, info_text_y + 32 * 2}, 28, 0, DARKGRAY);
				DrawTextEx(state.font[28], "W: toggle gradients", {info_text_x, info_text_y + 32 * 3}, 28, 0, DARKGRAY);
				DrawTextEx(state.font[28], "R: reset", {info_text_x, info_text_y + 32 * 4}, 28, 0, DARKGRAY);
			}
		} EndDrawing();
	}

	CloseWindow();
	particle_system_destroy(&state.particles);
	game_state_destroy(&state);
	return 0;
}
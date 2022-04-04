#include <iostream>
#include <SFML/Graphics.hpp>

#include "game_state.h"

int main()
{
	game_state state;
	memset(&state, 0, sizeof(game_state));
	
	// const char* cwd = std::filesystem::current_path().generic_string().c_str();
	// std::cout << "running from: " << cwd << "\n";

	i32 err = game_state_create_from_file(&state, "./grids/grid2.txt", 32);
	if(err != 0) {
		return err;
	}

	sf::RenderWindow window(sf::VideoMode(state.window_width, state.window_height), "grid algos");

	sf::RectangleShape cell_shape;
	cell_shape.setSize(sf::Vector2f((f32) state.grid_cell_size, (f32) state.grid_cell_size));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::P) {
					// TODO: run the algorithm
				}
			}
		}
	
		window.clear();

		// Input
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			// add wall	
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			sf::Vector2i cell_coord = window_to_grid_coord(&state, &state.grid, mouse_pos.x, mouse_pos.y);
			grid_set_cell_state(&state.grid, cell_coord.x, cell_coord.y, cell_state::STATE_WALL);
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			// remove wall	
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			sf::Vector2i cell_coord = window_to_grid_coord(&state, &state.grid, mouse_pos.x, mouse_pos.y);
			grid_set_cell_state(&state.grid, cell_coord.x, cell_coord.y, cell_state::STATE_EMPTY);
		}

		// Update

		// Drawing
		for(i32 y = 0; y < state.grid.height; ++y) {
			for(i32 x = 0; x < state.grid.width; ++x) {
				grid_cell cell = grid_get_cell(&state.grid, x, y);
				
				sf::Vector2f cell_pos = grid_to_window_coord(&state, &state.grid, x, y);
				cell_shape.setPosition(cell_pos);
				
				sf::Color cell_color = cell_state_get_color(cell.state);
				cell_shape.setFillColor(cell_color);
				
				window.draw(cell_shape);
			}	
		}
				
		window.display();
	}

	game_state_destroy(&state);
	return 0;
}
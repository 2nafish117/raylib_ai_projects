#include <iostream>
#include <SFML/Graphics.hpp>

#include "game_state.h"

int main()
{
	game_state state = {0};
	
	const char* cwd = std::filesystem::current_path().generic_string().c_str();
	std::cout << "running from: " << cwd << "\n";

	i32 err = game_state_create_from_file(&state, "./grids/grid2.txt", 32);
	if(err != 0) {
		return err;
	}

	sf::RenderWindow window(sf::VideoMode(state.window_width, state.window_height), "grid algos");

	sf::RectangleShape cell;
	cell.setSize(sf::Vector2f((f32) state.grid_cell_size, (f32) state.grid_cell_size));

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
			sf::Vector2i cell_coord = grid_get_cell_coord(&state, mouse_pos.x, mouse_pos.y);
			grid_set_value(&state, cell_coord.x, cell_coord.y, grid_state::STATE_WALL);
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			// remove wall	
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			sf::Vector2i cell_coord = grid_get_cell_coord(&state, mouse_pos.x, mouse_pos.y);
			grid_set_value(&state, cell_coord.x, cell_coord.y, grid_state::STATE_EMPTY);
		}

		// Update

		// Drawing
		for(i32 y = 0; y < state.grid_cell_count_y; ++y) {
			for(i32 x = 0; x < state.grid_cell_count_x; ++x) {
				grid_state grid_value = grid_get_cell_value(&state, x, y);
				
				sf::Vector2f cell_pos = grid_get_cell_pos(&state, x, y);
				cell.setPosition(cell_pos);
				
				sf::Color cell_color = grid_get_cell_color(grid_value);
				cell.setFillColor(cell_color);
				
				window.draw(cell);
			}	
		}
				
		window.display();
	}

	game_state_destroy(&state);
	return 0;
}
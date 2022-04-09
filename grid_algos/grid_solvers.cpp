#include "grid_solvers.h"

void grid_traverse_bfs(grid* g, grid_coord src, bool allow_diagonal) {

	std::queue<grid_coord> queue;

	// set all to unvisited
	grid_set_all_visited(g, false);
	//set all distance as INT_MAX
	grid_set_cell_distances(g, INT32_MAX);
	
	//set distance to source as 0
	grid_set_cell_distance(g, g->src, 0);
	grid_set_cell_visited(g, g->src, false);

	// push source into queue
	queue.push(src);

	while(!queue.empty()) {
		grid_coord coord = queue.front();
		queue.pop();

		// b8 curr_visited = grid_get_cell_visited(g, coord);
		i32 curr_distance = grid_get_cell_distance(g, coord);
		// cell_state curr_state = grid_get_cell_state(g, coord);

		grid_coord neighbour_coords[8] = {
			// vertical horizontal
			{coord.x, coord.y - 1},
			{coord.x, coord.y + 1},
			{coord.x - 1, coord.y},
			{coord.x + 1, coord.y},
			// diagonal
			{coord.x + 1, coord.y + 1},
			{coord.x + 1, coord.y - 1},
			{coord.x - 1, coord.y + 1},
			{coord.x - 1, coord.y - 1}
		};

		i32 num_neighbours = allow_diagonal ? 8 : 4;
		// for each neighbour check and push on queue
		for(i32 i = 0;i < num_neighbours; ++i) {
			grid_coord n = neighbour_coords[i];
			if(!grid_coord_valid(g, n)) {
				continue;
			}
			i32 n_distance = grid_get_cell_distance(g, n);
			b8 n_visited = grid_get_cell_visited(g, n);
			cell_state n_state = grid_get_cell_state(g, n);

			if(!n_visited && n_state != cell_state::BLOCKED) {
				i32 new_distance = MIN(n_distance, 1 + curr_distance);
				grid_set_cell_distance(g, n, new_distance);
				grid_set_cell_visited(g, n, true);
				queue.push(n);
			}
		}
	}
}
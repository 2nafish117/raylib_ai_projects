#include "grid_solvers.h"

void grid_traverse_bfs(grid* g, grid_coord src, bool allow_diagonal) {

	std::queue<grid_coord> queue;

	// set all to unvisited
	grid_set_all_cell_distances(g, INT32_MAX);
	
	//set distance to source as 0
	grid_set_cell_distance(g, g->src, 0);
	grid_set_cell_flags(g, g->src, );

	// push source into queue
	queue.push(src);

	while(!queue.empty()) {
		grid_coord coord = queue.front();
		queue.pop();

		grid_cell cell = grid_get_cell(g, coord);
		// if unvisited
		if(cell.state != cell_state::STATE_WALL) {
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
				if(n.x >= 0 && n.x < g->width && n.y >= 0 && n.y < g->height) {
					grid_cell ncell = grid_get_cell(g, n);

					if(ncell.state != cell_state::STATE_VISITED && ncell.state != cell_state::STATE_WALL) {
						i32 dist = MIN(ncell.distance, 1 + cell.distance);
						grid_set_cell_distance(g, n, dist);
						grid_set_cell_state(g, n, cell_state::STATE_VISITED);
						queue.push(n);
					}
				}
			}
		}
	}
}
#pragma once

#include "game_state.h"
#include <queue>

static INLINE bool is_safe(grid* g, i32 x, i32 y) {
	return y >= 0 && grid_get_cell(g, {x, y}).distance >= 0 && grid_get_cell(g, {x, y}).state != cell_state::STATE_WALL;
}

void grid_traverse_bfs(grid* g, cell_coord src, bool allow_diagonal = false) {

	std::queue<cell_coord> queue;

	// set all to unvisited
	grid_set_cell_distances(g, -1);
	
	//set distance to source as 0
	grid_set_cell_distance(g, {g->source_cell_x, g->source_cell_y}, 0);

	// push source into queue
	queue.push(src);

	while(!queue.empty()) {
		cell_coord coord = queue.front();
		queue.pop();

		grid_cell cell = grid_get_cell(g, coord);
		if(cell.distance < 0) {

			cell_coord neighbour_coords[8] = {
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
				cell_coord n = neighbour_coords[i];
				if(n.x >= 0 && n.x < g->width && n.y >= 0 && n.y < g->height) {
					grid_cell ncell = grid_get_cell(g, n);

					if(ncell.distance >= 0 && ncell.state != cell_state::STATE_WALL) {
						i32 dist = MIN(ncell.distance, 1 + cell.distance);
						grid_set_cell_distance(g, n, dist);
						queue.push(n);
					}
				}
			}
		}

	}
}
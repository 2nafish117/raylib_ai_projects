#pragma once

#include "game_state.h"
#include <queue>

void grid_traverse_bfs(grid* g, grid_coord src, bool allow_diagonal = false);
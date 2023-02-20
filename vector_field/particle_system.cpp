#include "particle_system.h"
#include "game_state.h"

void particle_system_create(particle_system* p, i32 particle_count) {
	p->particle_count = particle_count;
	p->particles = (particle*) malloc(particle_count * sizeof(particle));
	memset(p->particles, 0, particle_count * sizeof(particle));
}

void particle_system_destroy(particle_system* p) {
	free(p->particles);
	*p = {0};
}

void particle_system_randomize(particle_system* p, Vector2 top_left_bound, Vector2 bottom_right_bound) {
	for(i32 i = 0; i < p->particle_count; ++i) {
		p->particles[i].position.x = (f32) GetRandomValue((i32) top_left_bound.x, (i32) bottom_right_bound.x - 1);
		p->particles[i].position.y = (f32) GetRandomValue((i32) top_left_bound.y, (i32) bottom_right_bound.y - 1);
	}
}

static void particle_move_and_slide(const game_state* const state, const grid* const g, grid_coord coord, Vector2 start, Vector2 end, Vector2* position, Vector2* velocity) {
	// avoid infinity and 0 slopes
	if(end.x == start.x) {
		end.x += 0.001f;
	}
	if(end.y == start.y) {
		end.y += 0.001f;
	}

	// slope from start to end line segment
	f32 slope = (end.y - start.y) / (end.x - start.x);
	f32 cell_size = (f32) (state->grid_cell_size + state->grid_cell_padding);

	// rect of the cell
	Vector2 upper_left = grid_to_window_coord(state, g, coord);
	Vector2 lower_right = {upper_left.x + cell_size, upper_left.y + cell_size};
	
	// {
	// 	Vector2 grid_pixels_dim = grid_to_window_coord(state, g, {g->width - 1, g->height - 1});
	// 	// add a bit of padding
	// 	upper_left.x = CLAMP(upper_left.x - 10.0, 0.0, grid_pixels_dim.x);
	// 	upper_left.y = CLAMP(upper_left.y - 10.0, 0.0, grid_pixels_dim.y);
	// 	lower_right.x = CLAMP(lower_right.x + 10.0, 0.0, grid_pixels_dim.x);
	// 	lower_right.y = CLAMP(lower_right.y + 10.0, 0.0, grid_pixels_dim.y);
	// }

	// rect sorrounding end start points
	Vector2 bound_corner1 = {std::min(start.x, end.x), std::min(start.y, end.y)};
	Vector2 bound_corner2 = {std::max(start.x, end.x), std::max(start.y, end.y)};

	// intersection between cell rect and start to end
	Vector2 intersection = {0};
	{
		intersection.x = start.x + (upper_left.y - start.y) / slope;
		intersection.y = upper_left.y;
		bool intersection_in_bounds = intersection.x >= bound_corner1.x && intersection.x <= bound_corner2.x && intersection.y >= bound_corner1.y && intersection.x <= bound_corner2.x;
		if(intersection_in_bounds) {
			position->x = end.x;
			position->y = intersection.y - 0;
			velocity->y = 0.0;
			return;
		}
	}
	{
		intersection.x = lower_right.x;
		intersection.y = start.y + slope * (lower_right.x, start.x);
		bool intersection_in_bounds = intersection.x >= bound_corner1.x && intersection.x <= bound_corner2.x && intersection.y >= bound_corner1.y && intersection.x <= bound_corner2.x;
		if(intersection_in_bounds) {
			position->x = intersection.x + 0;
			position->y = start.y;
			velocity->x = 0.0;
			return;
		}
	}
	{
		intersection.x = start.x + (lower_right.y - start.y) / slope;
		intersection.y = lower_right.y;
		bool intersection_in_bounds = intersection.x >= bound_corner1.x && intersection.x <= bound_corner2.x && intersection.y >= bound_corner1.y && intersection.x <= bound_corner2.x;
		if(intersection_in_bounds) {
			position->x = end.x;
			position->y = intersection.y + 0;
			velocity->y = 0.0;
			return;
		}
	}
	{
		intersection.x = upper_left.x;
		intersection.y = start.y + slope * (upper_left.x, start.x);
		bool intersection_in_bounds = intersection.x >= bound_corner1.x && intersection.x <= bound_corner2.x && intersection.y >= bound_corner1.y && intersection.x <= bound_corner2.x;
		if(intersection_in_bounds) {
			position->x = intersection.x - 0;
			position->y = start.y;
			velocity->x = 0.0;
			return;
		}
	}
}

void particle_system_update(particle_system* p, const game_state* const state, const grid* const g, f32 steer_force, f32 dt) {
	for(i32 i = 0; i < p->particle_count; ++i) {
		grid_coord coord = window_to_grid_coord(state, g, p->particles[i].position);
		
		Vector2 old_velocity = p->particles[i].velocity;
		Vector2 old_position = p->particles[i].position;
		f32 speed = sqrtf(old_velocity.x * old_velocity.x + old_velocity.y * old_velocity.y);
		
		Vector2 gradient;
		if(grid_coord_valid(g, coord)) {
			gradient = grid_get_cell_gradient(g, coord);
		} else {
			Vector2 mid = grid_to_window_coord(state, g, {(i32) (g->width * 0.5), (i32) (g->height * 0.5)});
			gradient = {mid.x - old_position.x, mid.y - old_position.y};
			f32 len = sqrtf(gradient.x * gradient.x + gradient.y * gradient.y);
			len = len == 0 ? 1 : len;
			gradient.x /= len;
			gradient.y /= len;
		}
		if(grid_get_cell_state(g, coord) == cell_state::BLOCKED) {
			Vector2 mid = grid_to_window_coord(state, g, coord);
			mid.x += state->grid_cell_size + state->grid_cell_padding;
			mid.y += state->grid_cell_size + state->grid_cell_padding;
			gradient = {mid.x - old_position.x, mid.y - old_position.y};
			f32 len = sqrtf(gradient.x * gradient.x + gradient.y * gradient.y);
			len = len == 0 ? 1 : len;
			gradient.x /= len;
			gradient.y /= len;
		}

		const f32 max_speed = 300.0f;
		gradient.x *= max_speed;
		gradient.y *= max_speed;

		Vector2 steering = {gradient.x - old_velocity.x, gradient.y - old_velocity.y};
		steering.x *= steer_force;
		steering.y *= steer_force;

		Vector2 new_velocity = {old_velocity.x + steering.x * dt, old_velocity.y + steering.y * dt};
		Vector2 new_position = {old_position.x + new_velocity.x * dt, old_position.y + new_velocity.y * dt};
		// update position
		
		grid_coord new_coord = window_to_grid_coord(state, g, new_position);
		if(!grid_coord_valid(g, new_coord) || grid_get_cell_state(g, new_coord) != cell_state::EMPTY) {
			// move and slide particle on grid cell and return new_position, new_velocity after collision as out param 
			particle_move_and_slide(state, g, new_coord, old_position, new_position, &new_position, &new_velocity);
		}

		p->particles[i].velocity = new_velocity;
		p->particles[i].position = new_position;
	}
}
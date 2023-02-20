#pragma once

#include <raylib.h>
#include <common.h>

struct grid;
struct game_state;

struct particle {
	Vector2 position;
	Vector2 velocity;
	Color color;
};

struct particle_system {
	particle* particles;
	i32 particle_count;
};

void particle_system_create(particle_system* p, i32 particle_count);
void particle_system_destroy(particle_system* p);
void particle_system_randomize(particle_system* p, Vector2 top_left_bound, Vector2 bottom_right_bound);
void particle_system_update(particle_system* p, const game_state* state, const grid* const g, f32 steer_force, f32 dt);
#pragma once

#include "defines.h"

namespace math {
	typedef struct vec2 {
		union {
			f32 elements[2];
			struct { f32 x, y; };
			struct { f32 u, v; };
			struct { f32 s, t; };
			struct { f32 r, g; };
		};
	} vec2;

	typedef struct vec3 {
		union {
			f32 elements[3];
			struct { f32 x, y, z; };
			struct { f32 u, v, w; };
			struct { f32 s, t, p; };
			struct { f32 r, g, b; };
		};
	} vec3;

	typedef struct vec4 {
		union {
			f32 elements[4];
			struct { f32 x, y, z, w; };
			struct { f32 s, t, p, q; };
			struct { f32 r, g, b, a; };
		};
	} vec4;

	typedef vec4 quat;

	typedef struct mat4 {
		union {
			f32 data[16];
			vec4 rows[4];
		};
	} mat4;
} // namespace math
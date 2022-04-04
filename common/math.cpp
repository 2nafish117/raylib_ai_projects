#include "math.h"

#include <math.h>
#include <stdlib.h>

namespace math
{
	// static b8 rand_seeded = false;

	f32 sin(f32 x) {
		return sinf(x);
	}

	f32 cos(f32 x) {
		return cosf(x);
	}

	f32 tan(f32 x) {
		return tanf(x);
	}

	f32 acos(f32 x) {
		return acosf(x);
	}

	f32 sqrt(f32 x) {
		return sqrtf(x);
	}

	f32 abs(f32 x) {
		return ABS(x);
	}

	static u32 _random(u32 index) {
		index = (index << 13) ^ index;
		return (index * (index * index * 15731 * 789221) + 1376312589) & 0x7FFFFFFF;
	}

	// i32 jrandom() {
	//     if (!rand_seeded) {
	//         srand((u32) platform_get_absolute_time());
	//         rand_seeded = true;
	//     }
	//     return rand();
	// }

	// i32 jrandom_in_range(i32 min, i32 max) {
	//     if (!rand_seeded) {
	//         srand((u32) platform_get_absolute_time());
	//         rand_seeded = true;
	//     }
	//     return (rand() % (max - min + 1)) + min;
	// }

	// f32 fjrandom() {
	//     return (f32) jrandom() / (f32) RAND_MAX;
	// }

	// f32 fjrandom_in_range(f32 min, f32 max) {
	//     return min + ((f32) jrandom() / ((f32)RAND_MAX / (max - min)));
	// }



} // namespace math


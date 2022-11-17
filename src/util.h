//
// Created by Emmet on 11/9/2022.
//

#ifndef ALIFE_UTIL_H
#define ALIFE_UTIL_H

#include "types.h"

namespace Util {
	auto ceilDiv(u32 x, u32 y) -> u32;

	struct Coord {
		i32 x = 0;
		i32 y = 0;
	};

	auto positiveMod(i32 i, i32 n) -> i32;
}

#endif //ALIFE_UTIL_H

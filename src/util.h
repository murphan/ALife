//
// Created by Emmet on 11/9/2022.
//

#ifndef ALIFE_UTIL_H
#define ALIFE_UTIL_H

#include <vector>

#include "types.h"

namespace Util {
	auto ceilDiv(u32 x, u32 y) -> u32;

	struct Coord {
		i32 x;
		i32 y;

        Coord();
        Coord(i32, i32);
	};

	auto positiveMod(i32 i, i32 n) -> i32;

    auto printGrid(std::vector<i32> &, i32 width) -> void;
}

#endif //ALIFE_UTIL_H

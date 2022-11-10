//
// Created by Emmet on 11/9/2022.
//

#include "util.h"

auto Util::ceilDiv(u32 x, u32 y) -> u32 {
	return x / y + (x % y != 0);
}

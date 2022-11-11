//
// Created by Emmet on 11/9/2022.
//

#include "util.h"

auto Util::ceilDiv(u32 x, u32 y) -> u32 {
	return x / y + (x % y != 0);
}

auto Util::positiveMod(i32 i, i32 n) -> i32 {
	return (i % n + n) % n;
}
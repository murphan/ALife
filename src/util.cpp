//
// Created by Emmet on 11/9/2022.
//

#include <iostream>
#include <iomanip>

#include "util.h"

auto Util::ceilDiv(u32 x, u32 y) -> u32 {
	return x / y + (x % y != 0);
}

auto Util::positiveMod(i32 i, i32 n) -> i32 {
	return (i % n + n) % n;
}

Util::Coord::Coord(i32 x, i32 y): x(x), y(y) {}

Util::Coord::Coord(): x(0), y(0) {}

auto Util::printGrid(std::vector<i32> & grid, i32 width) -> void {
    auto height = grid.size() / width;

    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            std::cout << std::setfill('0') << std::setw(2) << grid[y * width + x] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
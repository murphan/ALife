//
// Created by Emmet on 11/9/2022.
//

#include <iostream>
#include <iomanip>

#include "util.h"

auto Util::ceilDiv(u32 x, u32 y) -> u32 {
	return x / y + (x % y != 0);
}

auto Util:: floorDiv(i32 x, u32 y) -> i32 {
	return x >= 0 ? x / (i32)y : x / (i32)y - (x % y != 0);
}

auto Util::outer(f32 x) -> i32 {
	return x > 0 ? (i32)ceil(x) : (i32)floor(x);
}

auto Util::positiveMod(i32 a, i32 b) -> i32 {
	return (a % b + b) % b;
}

auto Util::positiveMod(f32 a, f32 b) -> f32 {
	return fmod((fmod(a, b) + b), b);
}

Util::Coord::Coord(i32 x, i32 y): x(x), y(y) {}

Util::Coord::Coord(): x(0), y(0) {}

auto Util::Coord::operator+(const Util::Coord && other) const -> Util::Coord {
	return { x + other.x, y + other.y };
}

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

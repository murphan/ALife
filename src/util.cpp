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

/*
 * implementaion of base64 functions stolen from user Manuel Martinez on stackoverflow
 * https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
 */

auto Util::base64Encode(const std::string & in) -> std::string {
	std::string out;

	int val = 0, valb = -6;
	for (u8 current : in) {
		val = (val << 8) + current;
		valb += 8;
		while (valb >= 0) {
			out.push_back(BASE64_STRING[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back(BASE64_STRING[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');

	return out;
}

auto Util::base64Decode(const std::string & in) -> std::string {
	std::string out;

	std::vector<int> T(256,-1);
	for (int i = 0; i < 64; i++) T[BASE64_STRING[i]] = i;

	int val = 0, valb = -8;
	for (u8 current : in) {
		if (T[current] == -1) break;
		val = (val << 6) + T[current];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}

	return out;
}

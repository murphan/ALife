//
// Created by Emmet on 11/10/2022.
//

#include "direction.h"

Util::Coord Direction::table[8] = {
	{ 1, 0 },
	{ 1, 1 },
	{ 0, 1 },
	{ -1, 1 },
	{ -1, 0 },
	{ -1, -1 },
	{ 0, -1 },
	{ 1, -1 }
};

i32 Direction::normalizedTable[8] = {
	0,
	1,
	2,
	3,
	4,
	-3,
	-2,
	-1
};

Direction::Direction(i32 direction): direction(direction) {}

auto Direction::x() const -> i32 {
	return table[direction].x;
}

auto Direction::y() const -> i32 {
	return table[direction].y;
}

auto Direction::rotate(i32 relative) const -> Direction {
	return Direction(Util::positiveMod(direction + relative, 8));
}

auto Direction::rotate(Direction relative) const -> Direction {
	auto amount = normalizedTable[relative.direction];
	return Direction(Util::positiveMod(direction + amount, 8));
}

auto Direction::value() const -> i32 {
	return direction;
}

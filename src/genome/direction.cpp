//
// Created by Emmet on 11/10/2022.
//

#include "direction.h"

/**
 * lookup table for the x and y components of the
 * corresponding direction
 */
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

/**
 * converts a direction into an amount of ticks relative from RIGHT
 */
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

Direction::Direction(i32 direction): direction(Util::positiveMod(direction, 8)) {}

Direction::Direction() : direction(0) {}

/**
 * x component of a vector in this direction
 * has length either -1, 0, or 1
 */
auto Direction::x() const -> i32 {
	return table[direction].x;
}

/**
 * y component of a vector in this direction
 * has length either -1, 0, or 1
 */
auto Direction::y() const -> i32 {
	return table[direction].y;
}

/**
 * @return a new direction relative many ticks away, either positive or negative
 */
auto Direction::rotate(i32 relative) const -> Direction {
	return Direction(Util::positiveMod(direction + relative, 8));
}

/**
 * instead of specifying ticks to rotate by,
 * the passed direction is interpreted as a number of ticks away from RIGHT
 */
auto Direction::rotate(Direction relative) const -> Direction {
	auto amount = normalizedTable[relative.direction];
	return Direction(Util::positiveMod(direction + amount, 8));
}

auto Direction::value() const -> i32 {
	return direction;
}

auto Direction::normalized() const -> i32 {
	return normalizedTable[direction];
}
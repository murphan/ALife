//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_DIRECTION_H
#define ALIFE_DIRECTION_H

#include "types.h"
#include "util.h"

/**
 * represents a discrete direction with 8 possibilities
 * Direction of value 0 points right,
 * each next tick up in value is an additional 45 degrees
 * modulo 8: the direction after 7 is 0 and so on
 *
 * think of the point (cos( pi/4 a), sin( pi/4 a)) for a in [0..7]
 * except the resulting vector from the direction is rounded to the next integer (1) in the case of diagonals
 *
 * consult the values of [table] for the possible vectors
 */
class Direction {
private:
	static Util::Coord table[8];
	static i32 normalizedTable[8];

	i32 direction;

public:
	enum {
		RIGHT,
		RIGHT_UP,
		UP,
		LEFT_UP,
		LEFT,
		LEFT_DOWN,
		DOWN,
		RIGHT_DOWN,
	};

    Direction();
	Direction(i32);

	auto x() const -> i32;
	auto y() const -> i32;

	auto value() const -> i32;
	auto normalized() const -> i32;

	auto rotate(i32) const -> Direction;
	auto rotate(Direction) const -> Direction;

	auto isDiagonal() const -> bool;
};

#endif //ALIFE_DIRECTION_H

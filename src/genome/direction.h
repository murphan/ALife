//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_DIRECTION_H
#define ALIFE_DIRECTION_H

#include "types.h"
#include "util.h"

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

	explicit Direction(i32);

	auto x() const -> i32;
	auto y() const -> i32;

	auto value() const -> i32;

	auto rotate(i32) const -> Direction;
	auto rotate(Direction) const -> Direction;
};

#endif //ALIFE_DIRECTION_H

//
// Created by Emmet on 11/10/2022.
//

export module Direction;

import Coord;
import Types;
import Util;

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
export class Direction {
private:
	static Coord table[8];
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

	Direction() : direction(0) {};
	Direction(i32 direction) : direction(Util::positiveMod(direction, 8)) {};

	[[nodiscard]] auto x() const -> i32 {
		return table[direction].x;
	}

	[[nodiscard]] auto y() const -> i32 {
		return table[direction].y;
	}

	[[nodiscard]] auto value() const -> i32 {
		return direction;
	}

	[[nodiscard]] auto normalized() const -> i32 {
		return normalizedTable[direction];
	}

	[[nodiscard]] auto rotate(i32 relative) const -> Direction {
		return { Util::positiveMod(direction + relative, 8) };
	}

	[[nodiscard]] auto rotate(Direction relative) const -> Direction  {
		auto amount = normalizedTable[relative.direction];
		return { Util::positiveMod(direction + amount, 8) };
	}

	[[nodiscard]] auto isDiagonal() const -> bool {
		return direction == RIGHT_UP || direction == LEFT_UP || direction == LEFT_DOWN || direction == RIGHT_DOWN;
	}

	[[nodiscard]] auto opposite() const -> Direction {
		return { (direction + 4) % 8 };
	}
};


/**
 * lookup table for the x and y components of the
 * corresponding direction
 */
Coord Direction::table[8] = {
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

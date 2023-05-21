//
// Created by Emmet on 2/23/2023.
//

module;

#include <cmath>

export module Rotation;

import Util;
import Coord;
import Types;
import Direction;

auto emptyempty = Coord { 0, 0 };

Coord diagonalGrid[] = {
	emptyempty, emptyempty, emptyempty, { +2, -1 }, emptyempty, emptyempty, emptyempty,
	emptyempty, emptyempty, { +2, +0 }, { +2, -1 }, { +1, -2 }, emptyempty, emptyempty,
	emptyempty, { +1, +1 }, { +1, +1 }, { +1, +0 }, { +0, -1 }, { +0, -2 }, emptyempty,
	{ +1, +2 }, { +1, +1 }, { +1, +1 }, { +0, +0 }, { +0, -1 }, { -1, -2 }, { -1, -2 },
	emptyempty, { +0, +2 }, { +0, +1 }, { -1, +0 }, { -1, +0 }, { -2, -1 }, emptyempty,
	emptyempty, emptyempty, { -1, +2 }, { -2, +1 }, { -2, +0 }, emptyempty, emptyempty,
	emptyempty, emptyempty, emptyempty, { -2, +1 }, emptyempty, emptyempty, emptyempty,
};

auto getWithinDiamond(Coord xy) -> Coord {
	auto coordX = ((3.0f / 25.0f) * (f32)xy.x) + ((4.0f / 25.0f) * (f32)xy.y);
	auto coordY = ((3.0f / 25.0f) * (f32)xy.y) - ((4.0f / 25.0f) * (f32)xy.x);

	auto diamondX = (i32)floorf((coordX + 0.5f) / 1.0f);
	auto diamondY = (i32)floorf((coordY + 0.5f) / 1.0f);

	return { diamondX, diamondY };
}

auto getDiamondOffset(Coord diamond, Coord xy) -> Coord {
	auto centerX = diamond.x * 3 + diamond.y * -4;
	auto centerY = diamond.y * 3 + diamond.x * 4;

	auto offsetX = xy.x - centerX;
	auto offsetY = xy.y - centerY;

	return { offsetX, offsetY };
}

inline auto getDiamondGridOffset(Coord offset) -> Coord  {
	auto gridIndex = (6 - (offset.y + 3)) * 7 + (offset.x + 3);
	auto [subX, subY] = diagonalGrid[gridIndex];

	return { offset.x + subX, offset.y + subY };
}

export class Rotation {
public:
	static auto rotate90Step(Coord xy, i32 steps) -> Coord {
		switch (Util::positiveMod(steps, 4)) {
			case 0: return xy;
			case 1: return { xy.y, -xy.x };
			case 2: return { -xy.x, -xy.y };
			case 3:
			default: return { -xy.y, xy.x };
		}
	}

	static auto rotate45(Coord xy) -> Coord {
		auto diamond = getWithinDiamond(xy);

		auto offset = getDiamondOffset(diamond, xy);

		auto [subX, subY] = getDiamondGridOffset(offset);

		return { diamond.x * 5 + subX, diamond.y * 5 + subY };
	}

	static auto rotate(Coord xy, Direction direction) -> Coord {
		switch (direction.value()) {
			case Direction::RIGHT: return xy;
			case Direction::RIGHT_UP: return rotate90Step(rotate45(xy), 3);
			case Direction::UP: return rotate90Step(xy, 3);
			case Direction::LEFT_UP: return rotate90Step(rotate45(xy), 2);
			case Direction::LEFT: return rotate90Step(xy, 2);
			case Direction::LEFT_DOWN: return rotate90Step(rotate45(xy), 1);
			case Direction::DOWN: return rotate90Step(xy, 1);
			case Direction::RIGHT_DOWN:
			default: return rotate45(xy);
		}
	}

	static auto rotate(i32 x, i32 y, Direction direction) -> Coord {
		return rotate({ x, y }, direction);
	}
};

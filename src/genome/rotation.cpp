//
// Created by Emmet on 2/23/2023.
//

#include "rotation.h"

auto _____empty = Util::Coord { 0, 0 };

Util::Coord diagonalGrid[] = {
	_____empty, _____empty, _____empty, { +2, -1 }, _____empty, _____empty, _____empty,
	_____empty, _____empty, { +2, +0 }, { +2, -1 }, { +1, -2 }, _____empty, _____empty,
	_____empty, { +1, +1 }, { +1, +1 }, { +1, +0 }, { +0, -1 }, { +0, -2 }, _____empty,
	{ +1, +2 }, { +1, +1 }, { +1, +1 }, { +0, +0 }, { +0, -1 }, { -1, -2 }, { -1, -2 },
	_____empty, { +0, +2 }, { +0, +1 }, { -1, +0 }, { -1, +0 }, { -2, -1 }, _____empty,
	_____empty, _____empty, { -1, +2 }, { -2, +1 }, { -2, +0 }, _____empty, _____empty,
	_____empty, _____empty, _____empty, { -2, +1 }, _____empty, _____empty, _____empty,
};

inline auto getWithinDiamond(Util::Coord xy) -> Util::Coord {
	auto coordX = ((3.0f / 25.0f) * xy.x) + ((4.0f / 25.0f) * xy.y);
	auto coordY = ((3.0f / 25.0f) * xy.y) - ((4.0f / 25.0f) * xy.x);

	auto diamondX = (i32)floorf((coordX + 0.5f) / 1.0f);
	auto diamondY = (i32)floorf((coordY + 0.5f) / 1.0f);

	return { diamondX, diamondY };
}

inline auto getDiamondOffset(Util::Coord diamond, Util::Coord xy) -> Util::Coord {
	auto centerX = diamond.x * 3 + diamond.y * -4;
	auto centerY = diamond.y * 3 + diamond.x * 4;

	auto offsetX = xy.x - centerX;
	auto offsetY = xy.y - centerY;

	return { offsetX, offsetY };
}

inline auto getDiamondGridOffset(Util::Coord offset) -> Util::Coord  {
	auto gridIndex = (6 - (offset.y + 3)) * 7 + (offset.x + 3);
	auto [subX, subY] = diagonalGrid[gridIndex];

	return { offset.x + subX, offset.y + subY };
}

auto Rotation::rotate45(Util::Coord xy) -> Util::Coord {
	auto diamond = getWithinDiamond(xy);

	auto offset = getDiamondOffset(diamond, xy);

	auto [subX, subY] = getDiamondGridOffset(offset);

	return { diamond.x * 5 + subX, diamond.y * 5 + subY };
}

auto Rotation::rotate90Step(Util::Coord xy, i32 steps) -> Util::Coord {
	switch (Util::positiveMod(steps, 4)) {
		case 0: return xy;
		case 1: return { xy.y, -xy.x };
		case 2: return { -xy.x, -xy.y };
		case 3: return { -xy.y, xy.x };
	}
}

auto Rotation::rotate(Util::Coord xy, Direction direction) -> Util::Coord {
	switch (direction.value()) {
		case Direction::RIGHT: return xy;
		case Direction::RIGHT_UP: return rotate45(xy);
		case Direction::UP: return rotate90Step(xy, 1);
		case Direction::LEFT_UP: return rotate90Step(rotate45(xy), 1);
		case Direction::LEFT: return rotate90Step(xy, 2);
		case Direction::LEFT_DOWN: return rotate90Step(rotate45(xy), 2);
		case Direction::DOWN: return rotate90Step(xy, 3);
		case Direction::RIGHT_DOWN: return rotate90Step(rotate45(xy), 3);
	}
}
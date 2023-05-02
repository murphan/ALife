//
// Created by Emmet on 2/23/2023.
//

#ifndef ALIFE_ROTATION_H
#define ALIFE_ROTATION_H

#include "util.h"
#include "direction.h"

class Rotation {
public:
	static auto rotate90Step(Util::Coord xy, i32 steps) -> Util::Coord;

	static auto rotate45(Util::Coord xy) -> Util::Coord;

	static auto rotate(Util::Coord xy, Direction direction) -> Util::Coord;

	static auto rotate(i32 x, i32 y, Direction direction) -> Util::Coord;
};

#endif //ALIFE_ROTATION_H

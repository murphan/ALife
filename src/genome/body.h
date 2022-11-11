//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_BODY_H
#define ALIFE_BODY_H

#include <vector>

#include "types.h"
#include "util.h"
#include "direction.h"

class Body {
public:
	Body();
	Body(i32, i32, std::vector<i32>&&);

	i32 width;
	i32 height;
	std::vector<i32> body;

	auto getCell(i32, i32) -> i32;
};

class BodyBuilder {
private:
	auto expand(i32, i32) -> void;
	auto indexOf(i32, i32) const -> i32;

	/** borders */
	auto left() const -> i32;
	auto right() const -> i32;
	auto down() const -> i32;
	auto up() const -> i32;

public:
	i32 currentX, currentY;
	Direction currentDirection;

	i32 originX, originY;
	i32 width, height;

	std::vector<i32> canvas;
	Util::Coord anchors [4];

	BodyBuilder(i32, i32);

	auto addPart(Direction, i32, i32);

	auto setCurrentToAnchor(i32);

	auto toBody() const -> Body;
};

#endif //ALIFE_BODY_H

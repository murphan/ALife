//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_BODY_H
#define ALIFE_BODY_H

#include <vector>

#include "types.h"
#include "util.h"
#include "genome/direction.h"
#include "bodyPart.h"
#include "direction.h"

/**
 * additional context needed to build a body from instructions
 */
class BodyBuilder {
public:
    BodyBuilder();

    i32 currentX, currentY;
    Direction currentDirection;
    Util::Coord anchors [4];
};

class Body {
private:
    /** width of total canvas, not extent of organism body parts */
    i32 width, height;
    /** distance from bottom left of the canvas to the 0,0 point of the organism */
    i32 originX, originY;
    /** organism's body parts */
    std::vector<BodyPart> canvas;

	i32 numCells;

	/** bounds of the organism in the canvas, inclusive on both ends */
	i32 left, right, down, up;

	auto expand(i32, i32) -> void;
	auto indexOf(i32, i32) const -> i32;

    auto canvasLeft() const -> i32;
    auto canvasRight() const -> i32;
    auto canvasDown() const -> i32;
    auto canvasUp() const -> i32;

	auto safeAccess(i32 x, i32 y) const -> BodyPart;

public:
	Body(const Body & other) = default;
	Body(Body && other) = default;
	Body(i32 edge, BodyPart center);

	auto operator=(Body && other) noexcept -> Body & = default;

    auto accessExpand(i32, i32, i32) -> i32;
    auto access(i32, i32, Direction rotation) const -> BodyPart;

	auto addPart(BodyBuilder &, Direction, BodyPart part, i32 jumpAnchor) -> void;

    auto debugToString() const -> std::string;

	auto getNumCells() const -> i32;

	auto getWidth() const -> i32;
	auto getHeight() const -> i32;

	auto getLeft(Direction rotation) const -> i32;
	auto getRight(Direction rotation) const -> i32;
	auto getDown(Direction rotation) const -> i32;
	auto getUp(Direction rotation) const -> i32;
};

#endif //ALIFE_BODY_H

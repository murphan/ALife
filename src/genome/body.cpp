//
// Created by Emmet on 11/10/2022.
//

#include "body.h"

BodyBuilder::BodyBuilder() :
    currentX(0), currentY(0), currentDirection(Direction::RIGHT), anchors() {}

auto canvasIndex(i32 width, i32 x, i32 y) -> i32 {
	return y * width + x;
}

auto Body::expand(i32 expandX, i32 expandY) -> void {
	auto newWidth = width;
	auto newHeight = height;
	auto newOriginX = originX;
	auto newOriginY = originY;
	auto offX = 0;
	auto offY = 0;

	if (expandX < 0) {
		newWidth += -expandX;
		newOriginX += -expandX;
		offX = -expandX;

	} else if (expandX > 0) {
		newWidth += expandX;
	}

	if (expandY < 0) {
		newHeight += -expandY;
		newOriginY += -expandY;
		offY = -expandY;

	} else if (expandY > 0) {
		newHeight += expandY;
	}

	auto newCanvas = std::vector<BodyPart>(newWidth * newHeight, BodyPart::NONE);

	for (auto y = 0; y < height; ++y) {
		for (auto x = 0; x < width; ++x) {
			newCanvas[canvasIndex(newWidth, x + offX, y + offY)] = canvas[canvasIndex(width, x, y)];
		}
	}

	width = newWidth;
	height = newHeight;
	originX = newOriginX;
	originY = newOriginY;
	canvas = std::move(newCanvas);
}

auto Body::indexOf(i32 x, i32 y) const -> i32 {
	return (y + originY) * width + (x + originX);
}

auto Body::canvasLeft() const -> i32 {
    return -originX;
}

auto Body::canvasRight() const -> i32 {
    return (width - 1) - originX;
}

auto Body::canvasDown() const -> i32 {
    return -originY;
}

auto Body::canvasUp() const -> i32 {
    return (height - 1) - originY;
}

Body::Body(i32 edge, BodyPart center):
    width(edge * 2 + 1), height(edge * 2 + 1),
	originX(edge), originY(edge),
	canvas(width * height, BodyPart::NONE),
    left(0), right(0), down(0), up(0), numCells(0)
{
	canvas[indexOf(0, 0)] = center;
}

/**
 * @param builder the builder for the body creation process
 * @param direction relative direction from current direction to place body part
 * @param part the body segment to add
 * @param jumpAnchor use -1 for no usingAnchor, otherwise 0,1,2,3 for anchors A,B,C,D
 *
 * modifies the builder's current direction and current position
 */
auto Body::addPart(BodyBuilder & builder, Direction direction, BodyPart part, i32 jumpAnchor) -> void {
    /* move from current position unless anchored, then jump */
	auto baseX = builder.currentX;
	auto baseY = builder.currentY;

	if (jumpAnchor > -1) {
		baseX = builder.anchors[jumpAnchor].x;
		baseY = builder.anchors[jumpAnchor].y;
	}

    /* keep moving in direction until finding an empty space */
	auto newDirection = builder.currentDirection.rotate(direction);
    auto newX = baseX;
    auto newY = baseY;
    do {
        newX += newDirection.x();
        newY += newDirection.y();
    } while (accessExpand(newX, newY, 5) != 0);

	canvas[indexOf(newX, newY)] = part;

    /* update bounds */
    if (newX < left) left = newX;
    else if (newX > right) right = newX;

    if (newY < down) down = newY;
    else if (newY > up) up = newY;

    builder.currentDirection = newDirection;
	builder.currentX = newX;
	builder.currentY = newY;

	++numCells;
}

/** may resize the canvas if out of bounds */
auto Body::accessExpand(i32 x, i32 y, i32 expandBy) -> i32 {
    auto expandX = 0;
    auto expandY = 0;

    if (x < canvasLeft()) {
        expandX = -expandBy;
    } else if (x > canvasRight()) {
        expandX = expandBy;
    }
    if (y < canvasDown()) {
        expandY = -expandBy;
    } else if (y > canvasUp()) {
        expandY = expandBy;
    }
    if (expandX != 0 || expandY != 0) expand(expandX, expandY);

    return canvas[indexOf(x, y)];
}

auto Body::safeAccess(i32 x, i32 y) const -> BodyPart {
	if (x < canvasLeft() || x > canvasRight() || y < canvasDown() || y > canvasUp()) return BodyPart::NONE;
	return canvas[indexOf(x, y)];
}

using XY = Util::Coord;

Util::Coord block5RotationMap[25] = {
	XY(-3, 00), XY(-2, +1), XY(-1, +1), XY(-1, +2), XY(00, +3),
	XY(-2, -1), XY(-2, 00), XY(-1, 00), XY(00, +1), XY(00, +2),
	XY(-1, -2), XY(-1, -1), XY(00, 00), XY(+1, +1), XY(+1, +2),
	XY(-2, 00), XY(00, -1), XY(+1, -1), XY(+1, 00), XY(+2, +1),
	XY(-3, 00), XY(+1, -2), XY(+2, -1), XY(+2, 00), XY(+3, 00),
};

inline auto rotateCoord90(Util::Coord coord, Direction rotation) -> Util::Coord {
	switch (rotation.value()) {
		case Direction::RIGHT: return coord;
		case Direction::UP: return {-coord.y, coord.x };
		case Direction::LEFT: return {-coord.x, -coord.y };
		case Direction::DOWN: return {coord.y, -coord.x };
		default: throw std::exception("bad direction input");
	}
}

inline auto accessBlock5RotationMap(i32 blockX, i32 blockY, Direction rotation) -> Util::Coord {
	auto indexY = 4 - (blockY + 2), indexX = blockX + 2;
	return rotateCoord90(block5RotationMap[indexY * 5 + indexX], rotation.rotate(-1));
}

struct SuperSub {
	Util::Coord super;
	Util::Coord sub;
};

inline auto getSuperAndSubPosition(i32 x, i32 y) -> SuperSub {
	return {
		.super =  { Util::floorDiv(x + 2, 5), Util::floorDiv(y + 2, 5) },
		.sub =  { Util::positiveMod(x + 2, 5), Util::positiveMod(y + 2, 5) },
	};
}


inline auto superDiagonalOffsetX(i32 superX, i32 superY, Direction rotation) {
	switch (rotation.value()) {
		case Direction::RIGHT_UP: return superX * 3 + superY * -4;
		case Direction::LEFT_UP: return superX * -4 + superY * -3;
		case Direction::LEFT_DOWN: return superX * -3 + superY * 4;
		case Direction::RIGHT_DOWN: return superX * 4 + superY * 3;
		default: throw std::exception("bad direction input");
	}
}

inline auto superDiagonalOffsetY(i32 superX, i32 superY, Direction rotation) {
	switch (rotation.value()) {
		case Direction::RIGHT_UP: return superY * 3 + superX * 4;
		case Direction::LEFT_UP: return superY * -4 + superX * 3;
		case Direction::LEFT_DOWN: return superY * -3 + superX * -4;
		case Direction::RIGHT_DOWN: return superY * 4 + superX * -3;
		default: throw std::exception("bad direction input");
	}
}

inline auto rotateCoord45(Util::Coord coord, Direction rotation) -> Util::Coord {
	auto [super, sub] = getSuperAndSubPosition(coord.x, coord.y);

	auto superOffsetX = superDiagonalOffsetX(super.x, super.y, rotation);
	auto superOffsetY = superDiagonalOffsetY(super.x, super.y, rotation);

	auto [subOffsetX, subOffsetY] = accessBlock5RotationMap(sub.x, sub.y, rotation);

	return {superOffsetX + subOffsetX, superOffsetY + subOffsetY };
}

auto Body::access(i32 x, i32 y, Direction rotation) const -> BodyPart {
	if (rotation.isDiagonal()) {
		auto coord = rotateCoord45({ x, y }, rotation);
		return safeAccess(coord.x, coord.y);

	} else {
		auto coord = rotateCoord90({x, y}, rotation);
		return canvas[indexOf(coord.x, coord.y)];
	}
}

auto Body::debugToString() const -> std::string {
    auto string = std::string();
    string.reserve((width + 1) * height - 1);

    for (auto y = height - 1; y >= 0; --y) {
        for (auto x = 0; x < width; ++x) {
            string.push_back(canvas[canvasIndex(width, x, y)] + '0');
        }
        if (y > down) string.push_back('\n');
    }

    return string;
}

auto Body::getNumCells() const -> i32 {
	return numCells;
}

auto Body::getWidth() const -> i32 {
	return right - left + 1;
}

auto Body::getHeight() const -> i32 {
	return up - down + 1;
}

inline auto diagonalLength(i32 length0, i32 length1) {
	return Util::outer(sqrtf((f32)(length0 * length0 + length1 * length1)));
}

auto Body::getRight(Direction rotation) const -> i32 {
	switch (rotation.value()) {
		case Direction::RIGHT: return right;
		case Direction::RIGHT_UP: return diagonalLength(right, down);
		case Direction::UP: return -down;
		case Direction::LEFT_UP: return diagonalLength(down, left);
		case Direction::LEFT: return -left;
		case Direction::LEFT_DOWN: return diagonalLength(left, up);
		case Direction::DOWN: return up;
		case Direction::RIGHT_DOWN: return diagonalLength(up, right);
	}
}

auto Body::getUp(Direction rotation) const -> i32 {
	switch (rotation.value()) {
		case Direction::RIGHT: return up;
		case Direction::RIGHT_UP: return diagonalLength(up, right);
		case Direction::UP: return right;
		case Direction::LEFT_UP: return diagonalLength(right, down);
		case Direction::LEFT: return -down;
		case Direction::LEFT_DOWN: return diagonalLength(down, left);
		case Direction::DOWN: return -left;
		case Direction::RIGHT_DOWN: return diagonalLength(left, up);
	}
}

auto Body::getLeft(Direction rotation) const -> i32 {
	switch (rotation.value()) {
		case Direction::RIGHT: return left;
		case Direction::RIGHT_UP: return -diagonalLength(left, up);
		case Direction::UP: return -up;
		case Direction::LEFT_UP: return -diagonalLength(up, right);
		case Direction::LEFT: return -right;
		case Direction::LEFT_DOWN: return -diagonalLength(right, down);
		case Direction::DOWN: return down;
		case Direction::RIGHT_DOWN: return -diagonalLength(down, left);
	}
}

auto Body::getDown(Direction rotation) const -> i32 {
	switch (rotation.value()) {
		case Direction::RIGHT: return down;
		case Direction::RIGHT_UP: return -diagonalLength(down, left);
		case Direction::UP: return left;
		case Direction::LEFT_UP: return -diagonalLength(left, up);
		case Direction::LEFT: return -up;
		case Direction::LEFT_DOWN: return -diagonalLength(up, right);
		case Direction::DOWN: return -right;
		case Direction::RIGHT_DOWN: return -diagonalLength(right, down);
	}
}

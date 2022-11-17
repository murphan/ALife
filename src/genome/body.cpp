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

	auto newCanvas = std::vector<i32>(newWidth * newHeight);

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

Body::Body(i32 edge, i32 center):
    width(edge * 2 + 1), height(edge * 2 + 1),
	originX(edge), originY(edge),
	canvas(width * height),
    left(0), right(0), down(0), up(0)
{
	canvas[indexOf(0, 0)] = center;
}

/**
 * @param builder the builder for the body creation process
 * @param direction relative direction from current direction to place body part
 * @param part the body segment to add
 * @param anchor use -1 for no anchor, otherwise 0,1,2,3 for anchors A,B,C,D
 *
 * modifies the builder's current direction and current position
 */
auto Body::addPart(BodyBuilder & builder, Direction direction, i32 part, i32 anchor) -> void {
    /* move from current position unless anchored, then jump */
	auto baseX = builder.currentX;
	auto baseY = builder.currentY;

	if (anchor > 0) {
		baseX = builder.anchors[anchor].x;
		baseY = builder.anchors[anchor].x;
	}

    /* keep moving in direction until finding an empty space */
	auto newDirection = builder.currentDirection.rotate(direction);
    auto newX = baseX;
    auto newY = baseY;
    do {
        newX += newDirection.x();
        newY += newDirection.y();
    } while (accessExpand(newX, newY, 5) != 0);
;
	canvas[indexOf(newX, newY)] = part;

    /* update bounds */
    if (newX < left) left = newX;
    else if (newX > right) right = newX;

    if (newY < down) down = newY;
    else if (newY > up) up = newY;

    builder.currentDirection = newDirection;
	builder.currentX = newX;
	builder.currentY = newY;
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

auto Body::access(i32 x, i32 y) const -> i32 {
    return canvas[indexOf(x, y)];
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
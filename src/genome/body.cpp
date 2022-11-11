//
// Created by Emmet on 11/10/2022.
//

#include "body.h"

Body::Body(i32 width, i32 height, std::vector<i32> && body): width(width), height(height), body(std::move(body)) {}

auto Body::getCell(i32 x, i32 y) -> i32 {
	return body[y * width + x];
}

Body::Body(): width(0), height(0), body() {}

auto canvasIndex(i32 width, i32 x, i32 y) -> i32 {
	return y * width + x;
}

auto BodyBuilder::expand(i32 expandX, i32 expandY) -> void {
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

	auto newCanvas = std::vector<i32>(newWidth + newHeight);

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

auto BodyBuilder::indexOf(i32 x, i32 y) const -> i32 {
	return (y + originY) * width + (x + originX);
}

auto BodyBuilder::left() const -> i32 {
	return -originX;
}

auto BodyBuilder::right() const -> i32 {
	return (width - 1) - originX;
}

auto BodyBuilder::down() const -> i32 {
	return -originY;
}

auto BodyBuilder::up() const -> i32 {
	return (height - 1) - originY;
}

BodyBuilder::BodyBuilder(i32 edge, i32 center):
	currentX(0), currentY(0),
	currentDirection(Direction::RIGHT),
	originX(edge), originY(edge),
	width(edge * 2 + 1), height(edge * 2 + 1),
	canvas(width * height),
	anchors()
{
	anchors[0] = { 0, 0 };
	anchors[1] = { 0, 0 };
	anchors[1] = { 0, 0 };
	anchors[1] = { 0, 0 };

	canvas[indexOf(0, 0)] = center;
}

auto BodyBuilder::addPart(Direction direction, i32 part, i32 anchor) {
	auto baseX = currentX;
	auto baseY = currentY;

	if (anchor > 0) {
		baseX = anchors[anchor].x;
		baseY = anchors[anchor].x;
	}

	auto newDirection = currentDirection.rotate(direction);
	auto newX = baseX + newDirection.x();
	auto newY = baseY + newDirection.y();

	/* resize canvas if necessary */
	auto expandX = 0;
	auto expandY = 0;

	if (newX < left()) {
		expandX = -5;
	} else if (newX > right()) {
		expandX = 5;
	}
	if (newY < down()) {
		expandY = -5;
	} else if (newY > up()) {
		expandY = 5;
	}
	if (expandX != 0 && expandY != 0) expand(expandX, expandY);

	canvas[indexOf(newX, newY)] = part;

	currentDirection = newDirection;
	currentX = newX;
	currentY = newY;
}

auto BodyBuilder::setCurrentToAnchor(i32 anchor) {
	anchors[anchor] = { currentX, currentY };
}

auto BodyBuilder::toBody() const -> Body {
	/* shrinkwrap the canvas into the minimum box to fit the body */

	auto mostLeft = 0;
	auto mostRight = width - 1;
	auto mostDown = 0;
	auto mostUp = height - 1;

	for (auto x = 0; x < width; ++x) {
		for (auto y = 0; y < height; ++y) {
			if (canvas[y * width + x] != 0) {
				mostLeft = x;
				goto outer0;
			}
		}
	}
	outer0:;

	for (auto x = width - 1; x >= 0; --x) {
		for (auto y = 0; y < height; ++y) {
			if (canvas[y * width + x] != 0) {
				mostRight = x;
				goto outer1;
			}
		}
	}
	outer1:;

	for (auto y = 0; y < height; ++y) {
		for (auto x = 0; x < width; ++x) {
			if (canvas[y * width + x] != 0) {
				mostDown = y;
				goto outer2;
			}
		}
	}
	outer2:;

	for (auto y = height - 1; y >= 0; --y) {
		for (auto x = 0; x < width; ++x) {
			if (canvas[y * width + x] != 0) {
				mostUp = y;
				goto outer3;
			}
		}
	}
	outer3:;

	auto newWidth = mostRight - mostLeft + 1;
	auto newHeight = mostUp - mostDown + 1;

	auto body = std::vector<i32>(newWidth * newHeight);

	/* copy body over */
	for (auto y = 0; y < newHeight; ++y) {
		for (auto x = 0; x < newWidth; ++x) {
			body[y * newWidth + x] = canvas[(y + mostDown) * width + (x + mostLeft)];
		}
	}

	return { newWidth, newHeight, std::move(body) };
}
//
// Created by Emmet on 11/10/2022.
//

#include "body.h"
#include "rotation.h"

Body::Cell::Cell(
	BodyPart bodyPart_,
	i32 data_,
	i32 age_,
	bool dead_,
	i32 modifier_,
	bool broken_
): bodyPart_(bodyPart_), data_(data_), age_(age_), dead_(dead_), modifier_(modifier_), broken_(broken_) {}

auto Body::Cell::makeEmpty() -> Body::Cell {
	return Cell(BodyPart::NONE, 0, 0, false, -1, false);
}
auto Body::Cell::make(BodyPart bodyPart, i32 data, i32 age) -> Cell {
	return Cell(bodyPart, data, age, false, -1, false);
}

/* mutators */

auto Body::Cell::modify(i32 modifier) -> void {
	modifier_ = modifier + 1;
}
auto Body::Cell::setDead(bool dead) -> void {
	dead_ = dead;
}
auto Body::Cell::setAge(i32 age) -> void {
	age_ = age;
}

/* getters */

auto Body::Cell::bodyPart() const -> BodyPart {
	return bodyPart_;
}
auto Body::Cell::data() const -> i32 {
	return data_;
}
auto Body::Cell::isModified() const -> bool {
	return modifier_ != -1;
}
auto Body::Cell::modifier() const -> i32 {
	return modifier_ - 1;
}
auto Body::Cell::dead() const -> bool {
	return dead_;
}
auto Body::Cell::age() const -> i32 {
	return age_;
}

auto Body::Cell::cost(Settings & settings) const -> i32 {
	return settings.bodyPartCosts[bodyPart() - 1] + (isModified() ? 0 : settings.upgradedPartCosts[bodyPart() - 1]);
}

auto Body::Cell::empty() const -> bool {
	return bodyPart() == BodyPart::NONE;
}

auto Body::Cell::filled() const -> bool {
	return !empty();
}

auto Body::Cell::setBroken(bool broken) -> void {
	//broken_ = broken;
}

auto Body::Cell::broken() const -> bool {
	return false; broken_;
}

BodyBuilder::BodyBuilder() :
	currentX(0),
	currentY(0),
	anchors {
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
	} {}

auto BodyBuilder::getNextCellofType(BodyPart bodyPart, i32 & start) -> std::optional<Util::Coord> {
	for (auto i = start; i < insertedOrder.size(); ++i) {
		auto [current, x, y] = insertedOrder[i];
		if (current == bodyPart) {
			start = i;
			return std::make_optional<Util::Coord>(x, y);
		}
	}

	return std::nullopt;
}

auto BodyBuilder::add(BodyPart bodyPart, i32 x, i32 y) -> void {
	insertedOrder.push_back({ bodyPart, x, y });
}

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

	auto newCanvas = std::vector<Cell>(newWidth * newHeight, Cell::makeEmpty());

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

Body::Body(i32 edge):
    width(edge * 2 + 1), height(edge * 2 + 1),
	originX(edge), originY(edge),
	canvas(width * height, Cell::makeEmpty()),
    left(0), right(0), down(0), up(0)
{}

auto Body::outOfBounds = Cell::makeEmpty();

/**
 * @param builder the builder for the body creation process
 * @param direction direction to add the body part in
 * @param part the body segment to add
 * @param jumpAnchor use -1 for no usingAnchor, otherwise 0,1,2,3 for anchors A,B,C,D
 *
 * modifies the builder's current direction and current position
 */
auto Body::addCell(BodyBuilder & builder, Direction direction, Cell cell, i32 jumpAnchor) -> void {
    /* move from current position unless anchored, then jump */
	auto baseX = builder.currentX;
	auto baseY = builder.currentY;

	if (jumpAnchor > -1) {
		baseX = builder.anchors[jumpAnchor].x;
		baseY = builder.anchors[jumpAnchor].y;
	}

    /* keep moving in direction until finding an empty space */
    auto newX = baseX;
    auto newY = baseY;
    do {
        newX += direction.x();
        newY += direction.y();
    } while (accessExpand(newX, newY, 5).bodyPart() != BodyPart::NONE);

	builder.currentX = newX;
	builder.currentY = newY;

	directAddCell(builder, cell, newX, newY);
}

auto Body::directAddCell(BodyBuilder & builder, Cell cell, i32 x, i32 y) -> void {
	canvas[indexOf(x, y)] = cell;

	builder.add(cell.bodyPart(), x, y);

	/* update bounds */
	if (x < left) left = x;
	else if (x > right) right = x;

	if (y < down) down = y;
	else if (y > up) up = y;
}

/** may resize the canvas if out of bounds */
auto Body::accessExpand(i32 x, i32 y, i32 expandBy) -> Cell & {
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

auto Body::directAccess(i32 x, i32 y) -> Body::Cell & {
	return canvas[indexOf(x, y)];
}

auto Body::accessCoord(i32 x, i32 y, Direction rotation) -> AccessRecord {
	auto [accessX, accessY] = Rotation::rotate({ x, y }, rotation);
	if (accessX < canvasLeft() || accessX > canvasRight() || accessY < canvasDown() || accessY > canvasUp()) return { outOfBounds, accessX, accessY };
	return { canvas[indexOf(accessX, accessY)], accessX, accessY };
}

auto Body::access(i32 x, i32 y, Direction rotation) -> Cell & {
	auto [accessX, accessY] = Rotation::rotate({ x, y }, rotation);
	if (accessX < canvasLeft() || accessX > canvasRight() || accessY < canvasDown() || accessY > canvasUp()) return outOfBounds;
	return canvas[indexOf(accessX, accessY)];
}

auto Body::debugToString() const -> std::string {
    auto string = std::string();
    string.reserve((width + 1) * height - 1);

    for (auto y = height - 1; y >= 0; --y) {
        for (auto x = 0; x < width; ++x) {
            string.push_back(canvas[canvasIndex(width, x, y)].bodyPart() + '0');
        }
        if (y > down) string.push_back('\n');
    }

    return string;
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

//
// Created by Emmet on 11/10/2022.
//

#include "body.h"
#include "util/rotation.h"

Body::Cell::Cell(
	BodyPart bodyPart_,
	i32 age_,
	i32 modifier_,
	i16 x_,
	i16 y_,
	bool dead_,
	bool broken_
): bodyPart_(bodyPart_),
	age_(age_),
    modifier_(modifier_),
	x_(x_),
    y_(y_),
	dead_(dead_),
	broken_(broken_) {}

auto Body::Cell::makeEmpty() -> Body::Cell {
	return Cell(BodyPart::NONE, 0, -1, 0, 0, false, false);
}
auto Body::Cell::make(BodyPart bodyPart, i32 age) -> Cell {
	return Cell(bodyPart, age, -1, 0, 0, false, false);
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
	return settings.bodyPartCosts[bodyPart() - 1] + (isModified() ? settings.upgradedPartCosts[bodyPart() - 1] : 0);
}

auto Body::Cell::empty() const -> bool {
	return bodyPart() == BodyPart::NONE;
}

auto Body::Cell::filled() const -> bool {
	return !empty();
}

auto Body::Cell::setBroken(bool broken) -> void {
	broken_ = broken;
}

auto Body::Cell::broken() const -> bool {
	return broken_;
}

auto Body::Cell::setPosition(i32 x, i32 y) -> void {
	x_ = (i16)x;
	y_ = (i16)y;
}

auto Body::Cell::x() const -> i32 {
	return x_;
}

auto Body::Cell::y() const -> i32 {
	return y_;
}

/* Body */

Body::Body():
	cells(),
    left(0), right(0), down(0), up(0),
	currentX(0), currentY(0),
	anchors()
{}

auto Body::outOfBounds = Cell::makeEmpty();

/**
 * @param builder the builder for the body creation process
 * @param direction direction to add the body part in
 * @param part the body segment to add
 * @param jumpAnchor use -1 for no usingAnchor, otherwise the anchor id
 * @param setAnchor use -1 for not setting anchor, otherwise the anchor id to set here
 *
 * modifies the builder's current direction and current position
 */
auto Body::addCell(Direction direction, Cell cell, i32 jumpAnchor, i32 setAnchor) -> void {
    /* move from current position unless anchored, then jump */
	auto baseX = currentX;
	auto baseY = currentY;

	if (jumpAnchor > -1) {
		auto && anchor = popAnchorOfType(jumpAnchor);
		if (anchor.has_value()) {
			baseX = anchor->x;
			baseY = anchor->y;
		}
	}

    /* keep moving in direction until finding an empty space */
    auto newX = baseX;
    auto newY = baseY;
    do {
        newX += direction.x();
        newY += direction.y();
    } while (directAccess(newX, newY).filled());

	currentX = newX;
	currentY = newY;

	if (setAnchor != -1) {
		anchors.push_back({ currentX, currentY, setAnchor });
	}

	directAddCell(cell, newX, newY);
}

auto Body::directAddCell(Cell cell, i32 x, i32 y) -> void {
	cell.setPosition(x, y);
	cells.emplace_back(cell);

	/* update bounds */
	if (x < left) left = x;
	else if (x > right) right = x;

	if (y < down) down = y;
	else if (y > up) up = y;
}

auto Body::removeCell(Cell & cell) -> void {
	auto found = Util::find(cells, [&](Cell & entry) { return &cell == &entry; });
	//if (found == cells.end()) return;
	Util::quickErase(cells, found);
}

auto Body::getAnchorOfType(i32 type) -> Anchor * {
	for (auto && anchor : anchors) {
		if (anchor.type == type) return &anchor;
	}
	return nullptr;
}

auto Body::popAnchorOfType(i32 type) -> std::optional<Anchor> {
	auto found = Util::find(anchors, [&](Anchor & anchor) { return anchor.type == type; });
	if (found == anchors.end()) return std::nullopt;

	auto ret = std::make_optional(*found);

	Util::quickErase(anchors, found);

	return ret;
}

auto Body::directAccess(i32 x, i32 y) -> Body::Cell & {
	auto found = Util::find(cells, [&](Cell & cell) { return cell.x() == x && cell.y() == y; });
	if (found == cells.end()) return outOfBounds;
	return *found;
}

auto Body::getWidth() const -> i32 {
	return right - left + 1;
}

auto Body::getHeight() const -> i32 {
	return up - down + 1;
}

auto Body::getNextCellofType(BodyPart bodyPart, i32 & start) -> Cell * {
	for (auto i = start; i < cells.size(); ++i) {
		auto && cell = cells[i];
		if (cell.bodyPart() == bodyPart) {
			start = i;
			return &cell;
		}
	}

	return nullptr;
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
		case Direction::RIGHT_DOWN:
		default: return diagonalLength(up, right);
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
		case Direction::RIGHT_DOWN:
		default: return diagonalLength(left, up);
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
		case Direction::RIGHT_DOWN:
		default: return -diagonalLength(down, left);
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
		case Direction::RIGHT_DOWN:
		default: return -diagonalLength(right, down);
	}
}

auto Body::absoluteXY(Body::Cell cell, i32 centerX, i32 centerY, Direction rotation) -> Util::Coord {
	return Rotation::rotate(cell.x(), cell.y(), rotation) + Util::Coord { centerX, centerY };
}

auto Body::getCells() const -> const std::vector<Cell> & {
	return cells;
}

auto Body::getCells() -> std::vector<Cell> & {
	return cells;
}

auto Body::addAnchor(Anchor anchor) -> void {
	anchors.push_back(anchor);
}

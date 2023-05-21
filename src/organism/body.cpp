//
// Created by Emmet on 11/10/2022.
//

module;

#include <cmath>
#include <optional>
#include <vector>

export module Body;

import Types;
import Cell;
import BodyPart;
import Util;
import Direction;
import Rotation;
import Coord;

auto diagonalLength(i32 length0, i32 length1) {
	return Util::outer(sqrtf((f32)(length0 * length0 + length1 * length1)));
}

auto outOfBounds = Cell::makeEmpty();

export class Body {
private:
    /** width of total canvas, not extent of organism body parts */
	std::vector<Cell> cells;

	/** furthest extent of any cell, inclusive */
	i32 left, right, down, up;

	/** for building the body */
	i32 currentX, currentY;
	struct Anchor {
		i32 x, y;
		i32 type;
	};
	std::vector<Anchor> anchors;

	auto getAnchorOfType(i32 type) -> Anchor * {
		for (auto && anchor : anchors) {
			if (anchor.type == type) return &anchor;
		}
		return nullptr;
	}

	auto popAnchorOfType(i32 type) -> std::optional<Anchor> {
		auto found = Util::find(anchors, [&](Anchor & anchor) { return anchor.type == type; });
		if (found == anchors.end()) return std::nullopt;

		auto ret = std::make_optional(*found);

		Util::quickErase(anchors, found);

		return ret;
	}

public:
	Body(const Body & other) = default;
	Body(Body && other) = default;

	explicit Body() :
		cells(),
		left(0), right(0), down(0), up(0),
		currentX(0), currentY(0),
		anchors()
	{}

	auto operator=(Body && other) noexcept -> Body & = default;
	auto operator=(const Body & other) -> Body & = delete;

	auto directAccess(i32 x, i32 y) -> Cell & {
		auto found = Util::find(cells, [&](Cell & cell) { return cell.x() == x && cell.y() == y; });
		if (found == cells.end()) return outOfBounds;
		return *found;
	}

	auto addCell(Direction direction, Cell cell, i32 jumpAnchor, i32 setAnchor) -> void {
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

	auto directAddCell(Cell cell, i32 x, i32 y) -> void {
		cell.setPosition(x, y);
		cells.emplace_back(cell);

		/* update bounds */
		if (x < left) left = x;
		else if (x > right) right = x;

		if (y < down) down = y;
		else if (y > up) up = y;
	}

	auto removeCell(Cell & cell) -> void {
		auto found = Util::find(cells, [&](Cell & entry) { return &cell == &entry; });
		//if (found == cells.end()) return;
		Util::quickErase(cells, found);
	}

	auto getNextCellofType(BodyPart bodyPart, i32 & start) -> Cell * {
		for (auto i = start; i < cells.size(); ++i) {
			auto && cell = cells[i];
			if (cell.bodyPart() == bodyPart) {
				start = i;
				return &cell;
			}
		}

		return nullptr;
	}

	auto addAnchor(Anchor anchor) -> void {
		anchors.push_back(anchor);
	}

	[[nodiscard]] auto getWidth() const -> i32 {
		return right - left + 1;
	}
	[[nodiscard]] auto getHeight() const -> i32 {
		return up - down + 1;
	}

	[[nodiscard]] auto getLeft(Direction rotation) const -> i32  {
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

	[[nodiscard]] auto getRight(Direction rotation) const -> i32 {
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
	[[nodiscard]] auto getDown(Direction rotation) const -> i32 {
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

	[[nodiscard]] auto getUp(Direction rotation) const -> i32 {
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

	[[nodiscard]] auto getCells() const -> const std::vector<Cell> & {
		return cells;
	}

	auto getCells() -> std::vector<Cell> & {
		return cells;
	}

	static auto absoluteXY(Cell cell, i32 centerX, i32 centerY, Direction rotation) -> Coord {
		return Rotation::rotate(cell.x(), cell.y(), rotation) + Coord { centerX, centerY };
	}

};

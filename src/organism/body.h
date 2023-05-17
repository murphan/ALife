//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_BODY_H
#define ALIFE_BODY_H

#include <vector>
#include <optional>

#include "util/types.h"
#include "util/util.h"
#include "genome/direction.h"
#include "genome/bodyPart.h"
#include "genome/direction.h"
#include "settings.h"

class Body {
public:
	class Cell {
	private:
		BodyPart bodyPart_;
		i32 age_;
		i32 modifier_;
		i16 x_, y_;
		bool dead_;
		bool broken_;

	public:
		explicit Cell(
			BodyPart bodyPart_,
			i32 age_,
			i32 modifier_,
			i16 x_,
			i16 y_,
			bool dead_,
			bool broken_
		);

		static auto makeEmpty() -> Cell;
		static auto make(BodyPart bodyPart, i32 age) -> Cell;

		auto modify(i32 modifier) -> void;
		auto setDead(bool dead) -> void;
		auto setBroken(bool broken) -> void;
		auto setAge(i32 age) -> void;
		auto setPosition(i32 x, i32 y) -> void;

		[[nodiscard]] auto empty() const -> bool;
		[[nodiscard]] auto filled() const -> bool;
		[[nodiscard]] auto bodyPart() const -> BodyPart;
		[[nodiscard]] auto isModified() const -> bool;
		[[nodiscard]] auto modifier() const -> i32;
		[[nodiscard]] auto dead() const -> bool;
		[[nodiscard]] auto age() const -> i32;
		[[nodiscard]] auto broken() const -> bool;

		[[nodiscard]] auto x() const -> i32;
		[[nodiscard]] auto y() const -> i32;

		[[nodiscard]] auto cost(Settings & settings) const -> i32;
	};
private:
    /** width of total canvas, not extent of organism body parts */
    i32 width, height;
    /** distance from bottom left of the canvas to the 0,0 point of the organism */
    i32 originX, originY;
	std::vector<Cell> cells;

	/** bounds of the organism in the canvas, inclusive on both ends */
	i32 left, right, down, up;

	/** for building the body */
	i32 currentX, currentY;
	struct Anchor {
		i32 x, y;
		i32 type;
	};
	std::vector<Anchor> anchors;

	auto getAnchorOfType(i32 type) -> Anchor *;
	auto popAnchorOfType(i32 type) -> std::optional<Anchor>;

public:
	Body(const Body & other) = default;
	Body(Body && other) = default;

	explicit Body(i32 edge);

	/**
	 * I know this is unsafe but DO NOT modify this cell
	 */
	static Cell outOfBounds;

	auto operator=(Body && other) noexcept -> Body & = default;
	auto operator=(const Body & other) -> Body & = delete;

	auto directAccess(i32 x, i32 y) -> Cell &;

	auto addCell(Direction direction, Cell && cell, i32 jumpAnchor, i32 setAnchor) -> void;
	auto directAddCell(Cell && cell, i32 x, i32 y) -> void;
	auto removeCell(Cell & cell) -> void;
	auto getNextCellofType(BodyPart bodyPart, i32 & start) -> Cell *;

	auto getWidth() const -> i32;
	auto getHeight() const -> i32;

	auto getLeft(Direction rotation) const -> i32;
	auto getRight(Direction rotation) const -> i32;
	auto getDown(Direction rotation) const -> i32;
	auto getUp(Direction rotation) const -> i32;

	auto getCells() -> std::vector<Cell> &;

	static auto absoluteXY(Body::Cell & cell, i32 centerX, i32 centerY, Direction rotation) -> Util::Coord;
};

#endif //ALIFE_BODY_H

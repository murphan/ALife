//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_BODY_H
#define ALIFE_BODY_H

#include <vector>
#include <optional>

#include "types.h"
#include "util.h"
#include "genome/direction.h"
#include "bodyPart.h"
#include "direction.h"
#include "environment/settings.h"

/**
 * additional context needed to build a body from instructions
 */
class BodyBuilder {
public:
	struct Insertion {
		BodyPart bodyPart;
		i32 x, y;
	};
	std::vector<Insertion> insertedOrder;

    BodyBuilder();

    i32 currentX, currentY;
	Util::Coord anchors[4];

	auto add(BodyPart bodyPart, i32 x, i32 y) -> void;
	auto getNextCellofType(BodyPart bodyPart, i32 & start) -> std::optional<Util::Coord>;
};

class Body {
public:
	class Cell {
	private:
		BodyPart bodyPart_;
		i32 data_;
		i32 age_;
		bool dead_;
		i32 modifier_;
		bool broken_;

	public:
		explicit Cell(
			BodyPart bodyPart_,
			i32 data_,
			i32 age_,
			bool dead_,
			i32 modifier_,
			bool broken_
		);

		static auto makeEmpty() -> Cell;
		static auto make(BodyPart bodyPart, i32 data, i32 age) -> Cell;

		auto modify(i32 modifier) -> void;
		auto setDead(bool dead) -> void;
		auto setBroken(bool broken) -> void;
		auto setAge(i32 age) -> void;

		[[nodiscard]] auto empty() const -> bool;
		[[nodiscard]] auto filled() const -> bool;
		[[nodiscard]] auto bodyPart() const -> BodyPart;
		[[nodiscard]] auto data() const -> i32;
		[[nodiscard]] auto isModified() const -> bool;
		[[nodiscard]] auto modifier() const -> i32;
		[[nodiscard]] auto dead() const -> bool;
		[[nodiscard]] auto age() const -> i32;
		[[nodiscard]] auto broken() const -> bool;

		[[nodiscard]] auto cost(Settings & settings) const -> i32;
	};
private:
    /** width of total canvas, not extent of organism body parts */
    i32 width, height;
    /** distance from bottom left of the canvas to the 0,0 point of the organism */
    i32 originX, originY;
    /** organism's body parts */
    std::vector<Cell> canvas;

	/** bounds of the organism in the canvas, inclusive on both ends */
	i32 left, right, down, up;

	auto expand(i32, i32) -> void;
	auto indexOf(i32, i32) const -> i32;

    auto canvasLeft() const -> i32;
    auto canvasRight() const -> i32;
    auto canvasDown() const -> i32;
    auto canvasUp() const -> i32;

	auto accessExpand(i32, i32, i32) -> Cell &;

public:
	Body(const Body & other) = default;
	Body(Body && other) = default;
	Body(i32 edge);

	/**
	 * I know this is unsafe but DO NOT modify this cell
	 */
	static Cell outOfBounds;

	auto operator=(Body && other) noexcept -> Body & = default;

	struct AccessRecord {
		Cell & cell;
		i32 x, y;
	};
	auto accessCoord(i32, i32, Direction rotation) -> AccessRecord;
    auto access(i32, i32, Direction rotation) -> Cell &;
	auto directAccess(i32 x, i32 y) -> Cell &;

	auto addCell(BodyBuilder &, Direction, Cell cell, i32 jumpAnchor) -> void;
	auto directAddCell(BodyBuilder & builder, Cell cell, i32 x, i32 y) -> void;

    auto debugToString() const -> std::string;

	auto getWidth() const -> i32;
	auto getHeight() const -> i32;

	auto getLeft(Direction rotation) const -> i32;
	auto getRight(Direction rotation) const -> i32;
	auto getDown(Direction rotation) const -> i32;
	auto getUp(Direction rotation) const -> i32;
};

#endif //ALIFE_BODY_H

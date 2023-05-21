//
// Created by Emmet on 5/19/2023.
//

export module Cell;

import Types;
import BodyPart;
import Settings;

export class Cell {
private:
	BodyPart bodyPart_;
	i32 age_;
	i32 modifier_;
	i16 x_, y_;
	bool dead_;
	bool broken_;

public:
	constexpr explicit Cell(
		BodyPart bodyPart_,
		i32 age_,
		i32 modifier_,
		i16 x_,
		i16 y_,
		bool dead_,
		bool broken_
	) : bodyPart_(bodyPart_),
	    age_(age_),
	    modifier_(modifier_),
	    x_(x_),
	    y_(y_),
	    dead_(dead_),
	    broken_(broken_) {};

	constexpr static auto makeEmpty() -> Cell {
		return Cell(BodyPart::NONE, 0, -1, 0, 0, false, false);
	}

	constexpr static auto make(BodyPart bodyPart, i32 age) -> Cell  {
		return Cell(bodyPart, age, -1, 0, 0, false, false);
	}

	auto modify(i32 modifier) -> void {
		modifier_ = modifier + 1;
	}

	auto setDead(bool dead) -> void {
		dead_ = dead;
	}

	auto setBroken(bool broken) -> void {
		broken_ = broken;
	}

	auto setAge(i32 age) -> void {
		age_ = age;
	}

	auto setPosition(i32 x, i32 y) -> void {
		x_ = (i16)x;
		y_ = (i16)y;
	}

	[[nodiscard]] auto empty() const -> bool {
		return bodyPart() == BodyPart::NONE;
	}
	[[nodiscard]] auto filled() const -> bool {
		return !empty();
	}
	[[nodiscard]] auto bodyPart() const -> BodyPart {
		return bodyPart_;
	}
	[[nodiscard]] auto isModified() const -> bool {
		return modifier_ != -1;
	}
	[[nodiscard]] auto modifier() const -> i32 {
		return modifier_ - 1;
	}
	[[nodiscard]] auto dead() const -> bool {
		return dead_;
	}
	[[nodiscard]] auto age() const -> i32 {
		return age_;
	}
	[[nodiscard]] auto broken() const -> bool {
		return broken_;
	}
	[[nodiscard]] auto x() const -> i32 {
		return x_;
	}
	[[nodiscard]] auto y() const -> i32 {
		return y_;
	}

	[[nodiscard]] auto cost(Settings & settings) const -> i32 {
		return settings.bodyPartCosts[bodyPart() - 1] + (isModified() ? settings.upgradedPartCosts[bodyPart() - 1] : 0);
	}
};

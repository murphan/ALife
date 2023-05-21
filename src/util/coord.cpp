//
// Created by Emmet on 5/19/2023.
//
export module Coord;

import Types;

export class Coord {
public:
	i32 x;
	i32 y;

	Coord(): x(0), y(0) {};
	Coord(i32 x, i32 y): x(x), y(y) {};

	auto operator+(const Coord & other) const -> Coord {
		return { x + other.x, y + other.y };
	}
};

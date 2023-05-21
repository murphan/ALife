//
// Created by Emmet on 5/19/2023.
//

export module RenderBackground;

import Types;
import MapCell;

export class RenderBackground {
private:
	i32 width, height;
	constexpr static MapCell fakeMapCell = MapCell();

public:
	explicit RenderBackground(i32 width, i32 height): width(width), height(height) {};

	[[nodiscard]] static auto accessUnsafe(i32 x, i32 y) -> const MapCell & {
		return fakeMapCell;
	}

	[[nodiscard]] auto getWidth() const -> i32 {
		return width;
	}

	[[nodiscard]] auto getHeight() const -> i32 {
		return height;
	}
};

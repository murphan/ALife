//
// Created by Emmet on 5/19/2023.
//

#include "renderBackground.h"

auto RenderBackground::mapCell = MapCell();

RenderBackground::RenderBackground(i32 width, i32 height): width(width), height(height) {}

auto RenderBackground::accessUnsafe(i32 x, i32 y) -> const MapCell & {
	return mapCell;
}

auto RenderBackground::getWidth() const -> i32 {
	return width;
}

auto RenderBackground::getHeight() const -> i32 {
	return height;
}

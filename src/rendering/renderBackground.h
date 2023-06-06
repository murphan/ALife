//
// Created by Emmet on 5/19/2023.
//

#ifndef ALIFE_RENDERBACKGROUND_H
#define ALIFE_RENDERBACKGROUND_H

#include "util/types.h"
#include "environment/mapCell.h"

class RenderBackground {
private:
	i32 width, height;
	static MapCell mapCell;

public:
	explicit RenderBackground(i32 width, i32 height);

	[[nodiscard]] static auto accessUnsafe(i32 x, i32 y) -> const MapCell &;
	[[nodiscard]] auto getWidth() const -> i32;
	[[nodiscard]] auto getHeight() const -> i32;
};

#endif //ALIFE_RENDERBACKGROUND_H

//
// Created by Emmet on 2/27/2023.
//

#include "loop.h"

Loop::Loop(i32 fps) : fps(fps) {}

auto Loop::setFps(i32 fps) -> void {
	this->fps = fps;
}

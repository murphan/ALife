//
// Created by Emmet on 2/27/2023.
//

#include "loop.h"

Fps::Fps(i32 value): value(value) {}

auto Fps::isUnlimited() const -> bool {
	return value == -1;
}

auto Fps::unlimited() -> Fps {
	return Fps(-1);
}

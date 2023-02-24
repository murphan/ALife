//
// Created by Emmet on 2/23/2023.
//
#include "controls.h"

auto Controls::serialize() const -> json {
	return {
		{ "running", running },
		{ "fps", fps },
		{ "updateDisplay", updateDisplay }
	};
}


//
// Created by Emmet on 2/23/2023.
//

#include "controls.h"

auto Controls::serialize() const -> json {
	auto controls = json {
		{ "playing",       playing },
		{ "fps",           fps },
		{ "updateDisplay", updateDisplay },
		{ "displayMode", displayMode },
		{ "smartTree", smartTree },
		{ "doHighlight", doHighlight },
		{ "selectMode", selectMode },
	};

	if (activeNode == nullptr) {
		controls.push_back({ "activeNode", nullptr });
	} else {
		controls.push_back({ "activeNode", activeNode->uuid });
	}

	return controls;
}

auto Controls::unlimitedFPS() const -> bool {
	return fps >= 21;
}

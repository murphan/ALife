//
// Created by Emmet on 2/23/2023.
//

module;

#include "json.hpp"

export module Controls;

import Types;
import Node;

using json = nlohmann::json;

export class Controls {
public:
	enum DisplayMode {
		ENVIRONMENT,
		TREE,
	};

	enum SelectMode {
		SINGLE,
		LINEAGE,
	};

	bool playing;
	i32 fps;
	bool updateDisplay;
	Node * activeNode;
	DisplayMode displayMode;
	bool doHighlight;
	SelectMode selectMode;
	bool smartTree;

	[[nodiscard]] auto serialize() const -> json {
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

	[[nodiscard]] auto unlimitedFPS() const -> bool {
		return fps >= 21;
	}
};

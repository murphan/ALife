//
// Created by Emmet on 2/23/2023.
//

#ifndef ALIFE_CONTROLS_H
#define ALIFE_CONTROLS_H

#include <json.hpp>

#include "util/types.h"
#include "environment/node.h"

using json = nlohmann::json;

class Controls {
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

	[[nodiscard]] auto serialize() const -> json;

	[[nodiscard]] auto unlimitedFPS() const -> bool;
};

#endif //ALIFE_CONTROLS_H

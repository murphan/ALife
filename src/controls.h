//
// Created by Emmet on 2/23/2023.
//

#ifndef ALIFE_CONTROLS_H
#define ALIFE_CONTROLS_H

#include <json.hpp>

#include "util/types.h"
#include "environment/tree.h"

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
	Tree::Node * activeNode;
	DisplayMode displayMode;
	bool doHighlight;
	SelectMode selectMode;
	bool smartTree;

	auto serialize() const -> json;

	auto updateFromSerialized(json & input, Tree & tree) -> void;

	auto unlimitedFPS() const -> bool;
};

#endif //ALIFE_CONTROLS_H

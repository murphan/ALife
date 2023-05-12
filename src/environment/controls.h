//
// Created by Emmet on 2/23/2023.
//

#ifndef ALIFE_CONTROLS_H
#define ALIFE_CONTROLS_H

#include <json.hpp>

#include "types.h"
#include "tree.h"

using json = nlohmann::json;

class Controls {
public:
	enum DisplayMode {
		ENVIRONMENT,
		TREE,
	};

	bool playing;
	i32 fps;
	bool updateDisplay;
	Tree::Node * activeNode;
	DisplayMode displayMode;
	bool smartTree;

	auto serialize() const -> json;

	auto updateFromSerialized(json & input, Tree & tree) -> void;

	auto unlimitedFPS() const -> bool;
};

#endif //ALIFE_CONTROLS_H

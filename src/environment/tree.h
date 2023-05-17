//
// Created by Emmet on 5/4/2023.
//

#ifndef ALIFE_TREE_H
#define ALIFE_TREE_H

#include "util/types.h"
#include "genome/genome.h"
#include <json.hpp>
#include "controls.h"
#include "node.h"

using json = nlohmann::json;

class Tree {
private:
	u64 lastUUID;

public:
	std::vector<i32> levelTotals;
	std::unique_ptr<Node> root;

	explicit Tree();

	auto add(Node * parent, const Genome & newGenome) -> Node *;

	auto kill(Node * node, Node *& activeNode) -> void;

	auto update(Controls & controls) -> void;

	auto serialize(Node * activeNode) -> json;

	[[nodiscard]] auto getNodeByUUID(u32 uuid) const -> Node *;
};

#endif //ALIFE_TREE_H

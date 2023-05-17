//
// Created by Emmet on 5/4/2023.
//

#ifndef ALIFE_TREE_H
#define ALIFE_TREE_H

#include "util/types.h"
#include "genome/genome.h"
#include <json.hpp>

using json = nlohmann::json;

class Tree {
private:
	u64 lastUUID;

public:
	class Node {
	public:
		Genome genome;
		Node * parent;
		bool alive;
		std::vector<std::unique_ptr<Node>> children;

		u64 uuid;
		i32 value;
		i32 level;
		std::vector<i32> values;
		f32 hueLeft;
		f32 hueRight;
		bool active;

		explicit Node(Genome genome, u64 uuid);

		[[nodiscard]] auto isLeaf() const -> bool;
	};

	std::vector<i32> levelTotals;
	std::unique_ptr<Node> root;

	explicit Tree();

	auto add(Node * parent, const Genome & newGenome) -> Node *;

	auto kill(Node * node, Node *& activeNode) -> void;

	auto update(bool smart, bool updatePositions, Node * activeNode) -> void;

	auto serialize(Node * activeNode) -> json;

	[[nodiscard]] auto getNodeByUUID(u32 uuid) const -> Node *;
};

#endif //ALIFE_TREE_H

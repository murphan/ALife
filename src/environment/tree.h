//
// Created by Emmet on 5/4/2023.
//

#ifndef ALIFE_TREE_H
#define ALIFE_TREE_H

#include "types.h"
#include "genome/genome.h"
#include <json.hpp>

using json = nlohmann::json;

class Tree {
public:
	class Node {
	public:
		Genome genome;
		Node * parent;
		bool alive;
		std::vector<std::unique_ptr<Node>> children;

		i32 value;
		i32 level;
		std::vector<i32> values;
		f32 hueLeft;
		f32 hueRight;

		explicit Node(Genome genome);

		[[nodiscard]] auto isLeaf() const -> bool;
	};

	std::vector<i32> levelTotals;
	std::unique_ptr<Node> root;

	explicit Tree();

	auto add(Node * parent, const Genome & newGenome) -> Node *;

	auto kill(Node * node) -> void;

	auto computeValues(bool smart) -> void;

	auto serialize() -> json;
};

#endif //ALIFE_TREE_H

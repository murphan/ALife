//
// Created by Emmet on 5/16/2023.
//

#ifndef ALIFE_NODE_H
#define ALIFE_NODE_H

#include "util/types.h"
#include "genome/genome.h"
#include <memory>

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

#endif //ALIFE_NODE_H

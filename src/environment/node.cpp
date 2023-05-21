//
// Created by Emmet on 5/16/2023.
//

module;

#include <memory>
#include <vector>

export module Node;

import Types;
import Genome;

export class Node {
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

	explicit Node(Genome genome, u64 uuid):
		genome(std::move(genome)),
		parent(nullptr),
		alive(true),
		children(),
		uuid(uuid),
		value(0),
		level(0),
		values(),
		hueLeft(0),
		hueRight(0),
		active(false) {}

	[[nodiscard]] auto isLeaf() const -> bool {
		return children.empty();
	}
};

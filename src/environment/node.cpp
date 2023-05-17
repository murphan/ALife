//
// Created by Emmet on 5/16/2023.
//

#include "node.h"

Node::Node(Genome genome, u64 uuid):
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

auto Node::isLeaf() const -> bool {
	return children.empty();
}

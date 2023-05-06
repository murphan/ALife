//
// Created by Emmet on 5/4/2023.
//

#include "tree.h"
#include "util.h"

Tree::Tree(const Genome & genome): root(genome) {}

Tree::Node::Node(const Genome & genome): genome(genome), parent(nullptr), alive(true), children(), value(0), level(0), values() {}

auto Tree::Node::isRoot() const -> bool {
	return parent == nullptr;
}

auto Tree::Node::isLeaf() const -> bool {
	return children.empty();
}

auto Tree::Node::kill() -> void {
	alive = false;
	if (isRoot()) return;

	if (children.empty()) {
		auto && back = *parent;
		auto thisInBack = Util::find(back.children, [&](std::unique_ptr<Node> & descendant) { return descendant.get() == this; });
		Util::quickErase(back.children, thisInBack);

		if (!back.alive) {
			back.kill();
		}
	}
}

auto Tree::Node::addDescendent(const Genome & newGenome) -> Node * {
	children.emplace_back(std::make_unique<Node>(newGenome));
	auto && newNode = children.back();
	newNode->parent = this;
	return newNode.get();
}

struct QueueElement {
	json::reference parent_array;
	i32 index;
	Tree::Node * node;
};

auto Tree::serialize() -> json {
	computeValues();

	auto levelTotalsArray = json::array();
	for (auto && total : levelTotals) {
		levelTotalsArray.push_back(total);
	}

	auto tree = json {
		{ "levelTotals", levelTotalsArray }
	};

	auto stack = std::vector<QueueElement>();
	stack.push_back({ tree, -1, &root });

	while (!stack.empty()) {
		auto && [parent_array, index, node] = stack.back();
		auto && object = index == -1 ? parent_array : parent_array[index];

		stack.pop_back();

		object.push_back({ "value", node->value });
		object.push_back({ "level", node->level });
		object.push_back({ "children", json::array() });

		auto && child_array = object.find("children");

		for (auto && childNode : node->children) {
			child_array->push_back({});
			stack.push_back({*child_array, (i32)child_array->size() - 1, childNode.get() });
		}
	}

	return tree;
}

auto Tree::computeValues() -> void {
	auto iterStack = std::vector<Tree::Node *>();
	auto reverseStack = std::vector<Tree::Node *>();

	iterStack.push_back(&root);
	reverseStack.push_back(&root);
	root.level = 0;
	auto maxLevel = 0;

	while (!iterStack.empty()) {
		auto * node = iterStack.back();
		iterStack.pop_back();

		node->values.clear();
		if (node->level > maxLevel) maxLevel = node->level;

		for (auto && childNode : node->children) {
			childNode->level = node->level + 1;
			iterStack.push_back(childNode.get());
			reverseStack.push_back(childNode.get());
		}
	}

	levelTotals = std::vector(maxLevel + 1, 0);

	for (auto j = (i32)reverseStack.size() - 1; j >= 0; --j) {
		auto * node = reverseStack[j];

		node->values.resize(levelTotals.size(), 0);
		node->values[node->level] = 1;

		if (node->isLeaf()) {
			node->value = 1;
		} else {
			auto maxValue = 1;

			for (auto i = node->level + 1; i < levelTotals.size(); ++i) {
				for (auto && childNode : node->children) {
					node->values[i] += childNode->values[i];
				}
				if (node->values[i] > maxValue) maxValue = node->values[i];
			}

			node->value = maxValue;
		}

		levelTotals[node->level] += node->value;
	}
}
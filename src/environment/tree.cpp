//
// Created by Emmet on 5/4/2023.
//

#include "tree.h"
#include "util.h"

Tree::Tree(const Genome & genome): root(std::make_unique<Node>(genome)) {
	root->alive = false;
}

Tree::Node::Node(const Genome & genome): genome(genome), parent(nullptr), alive(true), children(), value(0), level(0), values() {}

auto Tree::Node::isRoot() const -> bool {
	return parent == nullptr;
}

auto Tree::Node::isLeaf() const -> bool {
	return children.empty();
}

auto Tree::kill(Node * node) -> void {
	auto * current = node;
	current->alive = false;

	while (!current->alive && !current->isRoot()) {
		auto * parent = current->parent;

		if (current->children.empty()) {
			auto currentInBack = Util::find(parent->children, [&](std::unique_ptr<Node> & child) { return child.get() == current; });
			Util::quickErase(parent->children, currentInBack);

		} else if (current->children.size() == 1) {
			auto && onlyChild = std::move(current->children.front());

			onlyChild->parent = parent;

			auto currentInBack = Util::find(parent->children, [&](std::unique_ptr<Node> & child) { return child.get() == current; });
			*currentInBack = std::move(onlyChild);
		}

		current = parent;
	}
}

auto Tree::Node::addDescendent(const Genome & newGenome) -> Node * {
	children.emplace_back(std::make_unique<Node>(newGenome));
	auto && newNode = children.back();
	newNode->parent = this;
	return newNode.get();
}

/* adapted from https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both */
/* user David H on Stackoverflow */

inline auto clamp(f32 value) -> f32 {
	if (value < 0.0_f32) return 0.0_f32;
	else if (value > 1.0_f32) return 1.0_f32;
	return value;
}

inline auto channelFloats2Int(f32 r, f32 g, f32 b) -> u32 {
	return ((u32)(clamp(r) * 255.0_f32) << 16) | ((u32)(clamp(g) * 255.0_f32) << 8) | (u32)(clamp(b) * 255.0_f32);
}

inline auto hsv2rgb(f32 h, f32 s, f32 v) -> u32 {
	if(s <= 0.0_f32)
		return channelFloats2Int(v, v, v);

	auto hh = fmod(h, 1.0_f32) * 6.0_f32;
	auto i = (i32)hh;
	auto ff = hh - (f32)i;
	auto p = v * (1.0_f32 - s);
	auto q = v * (1.0_f32 - (s * ff));
	auto t = v * (1.0_f32 - (s * (1.0_f32 - ff)));

	switch(i) {
		case 0:
			return channelFloats2Int(v, t, p);
		case 1:
			return channelFloats2Int(q, v, p);
		case 2:
			return channelFloats2Int(p, v, t);
		case 3:
			return channelFloats2Int(p, q, v);
		case 4:
			return channelFloats2Int(t, p, v);
		case 5:
		default:
			return channelFloats2Int(v, p, q);
	}
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
		{ "levelTotals", levelTotalsArray },
		{ "root", json::array() },
	};

	auto stack = std::vector<QueueElement>();
	stack.push_back({ *tree.find("root"), -1, root.get() });

	while (!stack.empty()) {
		auto && [parent_array, index, node] = stack.back();
		auto && object = index == -1 ? parent_array : parent_array[index];

		stack.pop_back();

		object.push_back(node->value);
		object.push_back(node->level);
		object.push_back(node->alive ? 1 : 0);
		object.push_back(hsv2rgb(
			(node->hueLeft + node->hueRight) / 2.0_f32,
			node->alive ? 1.0_f32 : 0.5_f32,
			node->alive ? 1.0_f32 : 0.5_f32
		));
		object.push_back(json::array());

		auto && child_array = object[4];

		for (auto && childNode : node->children) {
			child_array.push_back(json::array());
			stack.push_back({ child_array, (i32)child_array.size() - 1, childNode.get() });
		}
	}

	return tree;
}

inline auto interp(f32 along, f32 left, f32 right) -> f32 {
	return along * (right - left) + left;
}

auto Tree::computeValues() -> void {
	auto iterStack = std::vector<Tree::Node *>();
	auto reverseStack = std::vector<Tree::Node *>();

	iterStack.push_back(root.get());
	reverseStack.push_back(root.get());
	root->level = 0;
	root->hueLeft = 0.0_f32;
	root->hueRight = 1.0_f32;
	auto maxLevel = 0;

	while (!iterStack.empty()) {
		auto * node = iterStack.back();
		iterStack.pop_back();

		node->values.clear();
		if (node->level > maxLevel) maxLevel = node->level;

		for (auto i = 0; i < node->children.size(); ++i) {
			auto && childNode = node->children[i];

			childNode->level = node->level + 1;
			childNode->hueLeft = interp((f32)i / (f32)node->children.size(), node->hueLeft, node->hueRight);
			childNode->hueRight = interp((f32)(i + 1) / (f32)node->children.size(), node->hueLeft, node->hueRight);

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
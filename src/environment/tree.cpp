//
// Created by Emmet on 5/4/2023.
//

#include "tree.h"

#include <utility>
#include "util/util.h"
#include "util/color.h"

Tree::Tree(): lastUUID(0_u64), root(nullptr) {}

auto Tree::kill(Node * node, Node *& activeNode) -> void {
	auto * current = node;
	current->alive = false;

	while (!current->alive) {
		auto * parent = current->parent;

		if (parent == nullptr) {
			if (current->children.size() == 1) {
				auto && front = std::move(current->children.front());
				front->parent = nullptr;

				root = std::move(current->children.front());

				if (activeNode == current) activeNode = root.get();
			}

			return;

		} else if (current->children.empty()) {
			auto currentInBack = Util::find(parent->children, [&](std::unique_ptr<Node> & child) { return child.get() == current; });
			Util::quickErase(parent->children, currentInBack);

			if (activeNode == current) activeNode = nullptr;

		} else if (current->children.size() == 1) {
			auto && onlyChild = std::move(current->children.front());
			onlyChild->parent = parent;

			auto currentInBack = Util::find(parent->children, [&](std::unique_ptr<Node> & child) { return child.get() == current; });
			*currentInBack = std::move(onlyChild);

			if (activeNode == current) activeNode = currentInBack->get();
		}

		current = parent;
	}
}

auto Tree::add(Node * parent, const Genome & newGenome) -> Node * {
	auto nextUUID = lastUUID++;

	if (parent == nullptr) {
		root = std::make_unique<Node>(newGenome, nextUUID);
		return root.get();

	} else {
		parent->children.emplace_back(std::make_unique<Node>(newGenome, nextUUID));
		auto && newNode = parent->children.back();
		newNode->parent = parent;
		return newNode.get();
	}
}

struct QueueElement {
	json::reference parentArray;
	i32 indexInArray;
	Node * node;
};

auto Tree::serialize(Node * activeNode) -> json {
	auto levelTotalsArray = json::array();
	for (auto && total : levelTotals) {
		levelTotalsArray.push_back(total);
	}

	auto tree = json {
		{ "levelTotals", levelTotalsArray },
		{ "root", json::array() },
	};

	auto stack = std::vector<QueueElement>();
	if (root != nullptr) {
		stack.push_back({*tree.find("root"), -1, root.get()});
	}

	while (!stack.empty()) {
		auto && [parentArray, indexInArray, node] = stack.back();
		auto && object = indexInArray == -1 ? parentArray : parentArray[indexInArray];

		stack.pop_back();

		object.push_back(node->uuid);
		object.push_back(node->value);
		object.push_back(node->level);
		object.push_back(node->alive ? 1 : 0);
		object.push_back(node->active);
		object.push_back(activeNode == nullptr || node->active ? Color::hsv2rgb({
			(node->hueLeft + node->hueRight) / 2.0_f32,
			node->alive ? 1.0_f32 : 0.5_f32,
			node->alive ? 1.0_f32 : 0.5_f32
		}) : 0x4f4f4f);
		object.push_back(json::array());

		auto && child_array = object[6];

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

auto Tree::update(Controls & controls) -> void {
	if (root == nullptr) return;

	auto updatePositions = controls.displayMode == Controls::DisplayMode::TREE;

	auto iterStack = std::vector<Node *>();
	auto reverseStack = std::vector<Node *>();

	iterStack.push_back(root.get());
	reverseStack.push_back(root.get());
	root->level = 0;
	root->hueLeft = 0.0_f32;
	root->hueRight = 1.0_f32;
	root->active = false;
	auto maxLevel = 0;

	while (!iterStack.empty()) {
		auto * node = iterStack.back();
		iterStack.pop_back();

		node->values.clear();
		if (node->level > maxLevel) maxLevel = node->level;

		if (node == controls.activeNode) {
			node->active = true;
		} else {
			node->active = controls.selectMode == Controls::SelectMode::LINEAGE &&
				node->parent != nullptr && node->parent->active;
		}

		for (auto i = 0; i < node->children.size(); ++i) {
			auto && childNode = node->children[i];

			childNode->level = node->level + 1;
			childNode->hueLeft = interp((f32)i / (f32)node->children.size(), node->hueLeft, node->hueRight);
			childNode->hueRight = interp((f32)(i + 1) / (f32)node->children.size(), node->hueLeft, node->hueRight);

			iterStack.push_back(childNode.get());
			if (updatePositions) reverseStack.push_back(childNode.get());
		}
	}

	if (!updatePositions) return;

	levelTotals = std::vector(maxLevel + 1, 0);

	for (auto j = (i32)reverseStack.size() - 1; j >= 0; --j) {
		auto * node = reverseStack[j];

		if (controls.smartTree) {
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
		} else {
			node->value = 1;
			levelTotals[node->level] += 1;
		}
	}
}

auto Tree::getNodeByUUID(u32 uuid) const -> Node * {
	if (root == nullptr) return nullptr;

	auto stack = std::vector<Node *>(1, root.get());

	while (!stack.empty()) {
		auto * node = stack.back();
		stack.pop_back();

		if (node->uuid == uuid) return node;

		for (auto && child : node->children) stack.push_back(child.get());
	}

	return nullptr;
}

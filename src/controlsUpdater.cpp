//
// Created by Emmet on 5/16/2023.
//

module;

#include "json.hpp"

export module ControlsUpdater;

import Util;
import Controls;
import Types;
import Tree;

namespace ControlsUpdater {
	using json = nlohmann::json;

	template<Util::Function<void, bool> T>
	auto withBoolField(json & jsonObject, const char * fieldName, T func) -> void {
		if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_boolean()) {
			func(jsonObject[fieldName].get<bool>());
		}
	}

	template<typename Integer, Util::Function<void, Integer> T>
	auto withIntField(json & jsonObject, const char * fieldName, T func) -> void {
		static_assert(std::is_integral<Integer>::value, "Integral required");

		if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_number()) {
			func(jsonObject[fieldName].get<Integer>());
		}
	}

	template<Util::Function<void> T>
	auto withNullField(json & jsonObject, const char * fieldName, T func) -> void {
		if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_null()) {
			func();
		}
	}

	export auto update(json &input, Controls &controls, Tree & tree) -> void {
		if (!input.is_object()) return;

		withBoolField(input, "playing", [&](bool result) {
			controls.playing = result;
		});

		withIntField<i32>(input, "fps", [&](i32 result) {
			if (result < 0) result = 0;
			if (result > 21) result = 21;
			controls.fps = result;
		});

		withBoolField(input, "updateDisplay", [&](bool result) {
			controls.updateDisplay = result;
		});

		withIntField<u64>(input, "activeNode", [&](u64 result) {
			auto *found = tree.getNodeByUUID(result);
			if (found != nullptr) controls.activeNode = found;
		});
		withNullField(input, "activeNode", [&]() {
			controls.activeNode = nullptr;
		});

		withIntField<i32>(input, "displayMode", [&](i32 result) {
			if (result < 0) result = 0;
			if (result > 1) result = 1;
			controls.displayMode = (Controls::DisplayMode) result;
		});

		withBoolField(input, "smartTree", [&](bool result) {
			controls.smartTree = result;
		});

		withBoolField(input, "doHighlight", [&](bool result) {
			controls.doHighlight = result;
		});

		withIntField<i32>(input, "selectMode", [&](i32 result) {
			if (result < 0) result = 0;
			if (result > 1) result = 1;
			controls.selectMode = (Controls::SelectMode) result;
		});
	}
}

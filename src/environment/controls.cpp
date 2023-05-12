//
// Created by Emmet on 2/23/2023.
//

#include "controls.h"

#include "util.h"

auto Controls::serialize() const -> json {
	auto controls = json {
		{ "playing",       playing },
		{ "fps",           fps },
		{ "updateDisplay", updateDisplay },
		{ "displayMode", displayMode },
		{ "smartTree", smartTree },
	};

	if (activeNode == nullptr) {
		controls.push_back({ "activeNode", nullptr });
	} else {
		controls.push_back({ "activeNode", activeNode->uuid });
	}

	return controls;
}

template<Util::Function<void, bool> T>
inline auto withBoolField(json & jsonObject, const char * fieldName, T func) -> void {
	if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_boolean()) {
		func(jsonObject[fieldName].get<bool>());
	}
}

template<typename Integer, Util::Function<void, Integer> T>
inline auto withIntField(json & jsonObject, const char * fieldName, T func) -> void {
	static_assert(std::is_integral<Integer>::value, "Integral required");

	if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_number()) {
		func(jsonObject[fieldName].get<Integer>());
	}
}

template<Util::Function<void> T>
inline auto withNullField(json & jsonObject, const char * fieldName, T func) -> void {
	if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_null()) {
		func();
	}
}

auto Controls::updateFromSerialized(json & input, Tree & tree) -> void {
	if (!input.is_object()) return;

	withBoolField(input, "playing", [this](bool result) {
		playing = result;
	});

	withIntField<i32>(input, "fps", [this](i32 result) {
		if (result < 0) result = 0;
		if (result > 21) result = 21;
		fps = result;
	});

	withBoolField(input, "updateDisplay", [this](bool result) {
		updateDisplay = result;
	});

	withIntField<u64>(input, "activeNode", [&, this](u64 result) {
		auto * found = tree.getNodeByUUID(result);
		if (found != nullptr) activeNode = found;
	});
	withNullField(input, "activeNode", [this]() {
		activeNode = nullptr;
	});

	withIntField<i32>(input, "displayMode", [this](i32 result) {
		if (result < 0) result = 0;
		if (result > 1) result = 1;
		displayMode = (DisplayMode)result;
	});

	withBoolField(input, "smartTree", [this](bool result) {
		smartTree = result;
	});
}

auto Controls::unlimitedFPS() const -> bool {
	return fps >= 21;
}


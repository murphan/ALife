//
// Created by Emmet on 2/23/2023.
//

#include "controls.h"

#include "util.h"

auto Controls::serialize() const -> json {
	return {
		{ "playing",       playing },
		{ "fps",           fps },
		{ "updateDisplay", updateDisplay }
	};
}

template<Util::Function<bool, void> T>
inline auto withBoolField(json & jsonObject, const char * fieldName, T func) -> void {
	if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_boolean())
		func(jsonObject[fieldName].get<bool>());
}

template<Util::Function<i32, void> T>
inline auto withIntField(json & jsonObject, const char * fieldName, T func) -> void {
	if (jsonObject.contains(fieldName) && jsonObject[fieldName].is_number())
		func(jsonObject[fieldName].get<i32>());
}

auto Controls::updateFromSerialized(json & input) -> void {
	if (!input.is_object()) return;

	withBoolField(input, "playing", [this](bool result) {
		playing = result;
	});

	withIntField(input, "fps", [this](i32 result) {
		if (result < 0) result = 0;
		if (result > 200) result = 200;
		fps = result;
	});

	withBoolField(input, "updateDisplay", [this](bool result) {
		updateDisplay = result;
	});
}

auto Controls::unlimitedFPS() const -> bool {
	return fps >= 200;
}


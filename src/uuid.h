//
// Created by Emmet on 2/10/2023.
//

#ifndef ALIFE_UUID_H
#define ALIFE_UUID_H

#include <random>
#include <optional>

#include "types.h"

class UUID {
private:
	static std::random_device randomDevice;
	static std::default_random_engine randomEngine;
	static std::uniform_int_distribution<u64> randomRange;

	constexpr static const char * HEX_DIGITS = "0123456789abcdef";

	u64 high, low;

public:
	UUID(u64 high, u64 low);

	static auto generateRandom() -> UUID;
	static auto fromString(std::string & input) -> std::optional<UUID>;

	auto operator==(const UUID & other) const -> bool;

	auto asString() const -> std::string;

	auto getHigh() const -> u64;
	auto getLow() const -> u64;
};

#endif //ALIFE_UUID_H

//
// Created by Emmet on 2/10/2023.
//

#include "uuid.h"

#include <stdint.h>
#include <sstream>

auto UUID::randomDevice = std::random_device();
auto UUID::randomEngine = std::default_random_engine(randomDevice());
auto UUID::randomRange = std::uniform_int_distribution<u64>(0, UINT64_MAX);

UUID::UUID(u64 high, u64 low): high(high), low(low) {}

auto UUID::generateRandom() -> UUID {
	return {randomRange(randomEngine), randomRange(randomEngine)};
}

auto UUID::operator==(const UUID & other) const -> bool {
	return high == other.high && low == other.low;
}

auto UUID::asString() const -> std::string {
	auto str = std::string();
	str.reserve(32);

	for (auto i = 0; i < 16; ++i) {
		str.push_back(HEX_DIGITS[(high >> ((15 - i) * 4)) & 0xf]) ;
	}

	for (auto i = 0; i < 16; ++i) {
		str.push_back(HEX_DIGITS[(low >> ((15 - i) * 4)) & 0xf]);
	}

	return str;
}

auto UUID::getHigh() const -> u64 {
	return high;
}

auto UUID::getLow() const -> u64 {
	return low;
}

inline auto parseU64(std::string_view input) -> std::optional<u64> {
	auto value = 0_u64;

	for (auto && digit : input) {
		if (digit >= '0' && digit <= '9') {
			value = (value << 4) | (digit - '0');
		} else if (digit >= 'a' && digit <= 'f') {
			value = (value << 4) | (digit - 'a' + 10);
		} else {
			return std::nullopt;
		}
	}

	return std::make_optional(value);
}

auto UUID::fromString(std::string & input) -> std::optional<UUID> {
	if (input.length() != 32) return std::nullopt;

	auto highValue = parseU64(std::string_view(input.begin(), input.begin() + 16));
	if (!highValue.has_value()) return std::nullopt;

	auto lowValue = parseU64(std::string_view(input.begin() + 16, input.end()));
	if (!lowValue.has_value()) return std::nullopt;

	return UUID(highValue.value(), lowValue.value());
}

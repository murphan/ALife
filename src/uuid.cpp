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
	return UUID(randomRange(randomEngine), randomRange(randomEngine));
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

auto UUID::getHigh() -> u64 {
	return high;
}

auto UUID::getLow() -> u64 {
	return low;
}
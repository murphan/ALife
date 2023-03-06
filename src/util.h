//
// Created by Emmet on 11/9/2022.
//

#ifndef ALIFE_UTIL_H
#define ALIFE_UTIL_H

#include <vector>
#include <functional>
#include "types.h"

namespace Util {
	auto ceilDiv(u32 x, u32 y) -> u32;
	auto floorDiv(i32 x, u32 y) -> i32;

	auto outer(f32 x) -> i32;

	struct Coord {
		i32 x;
		i32 y;

        Coord();
		Coord(i32, i32);
	};

	auto positiveMod(i32 i, i32 n) -> i32;

    auto printGrid(std::vector<i32> &, i32 width) -> void;

	constexpr const char * BASE64_STRING = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	auto base64Encode(const std::string & in) -> std::string;
	auto base64Decode(const std::string & in) -> std::string;

	template<typename T, typename Out, typename ...In>
	concept Function = requires(T functionLike, In ...param) {
		{functionLike(param...)} -> std::same_as<Out>;
	};

	template<typename Iterator, typename ValueType>
	concept IsIterator = std::same_as<typename std::iterator_traits<Iterator>::value_type, ValueType>;
}

#endif //ALIFE_UTIL_H

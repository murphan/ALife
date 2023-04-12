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

	/*
     * implementaion of base64 functions stolen from user Manuel Martinez on stackoverflow
     * https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c
	 */

	template<typename T>
	auto base64Encode(const T & in) -> std::string {
		std::string out;

		int val = 0, valb = -6;
		for (u8 current : in) {
			val = (val << 8) + current;
			valb += 8;
			while (valb >= 0) {
				out.push_back(BASE64_STRING[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back(BASE64_STRING[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');

		return out;
	}

	template<typename T>
	auto base64Decode(const std::string & in) -> std::string {
		std::string out;

		std::vector<int> T(256,-1);
		for (int i = 0; i < 64; i++) T[BASE64_STRING[i]] = i;

		int val = 0, valb = -8;
		for (u8 current : in) {
			if (T[current] == -1) break;
			val = (val << 6) + T[current];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}

		return out;
	}

	template<typename T, typename Out, typename ...In>
	concept Function = requires(T functionLike, In ...param) {
		{functionLike(param...)} -> std::same_as<Out>;
	};

	template<typename Iterator, typename ValueType>
	concept IsIterator = std::same_as<typename std::iterator_traits<Iterator>::value_type, ValueType>;
}

#endif //ALIFE_UTIL_H

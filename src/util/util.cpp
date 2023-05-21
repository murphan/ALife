//
// Created by Emmet on 11/9/2022.
//

module;

#include <cmath>
#include <concepts>
#include <vector>
#include <iostream>
#include <iomanip>

export module Util;

import Types;

export namespace Util {
	template<typename T, typename Out, typename ...In>
	concept Function = requires(T functionLike, In ...param) {
		{functionLike(param...)} -> std::same_as<Out>;
	};

	template<typename Iterator, typename ValueType>
	concept IsIterator = std::same_as<typename std::iterator_traits<Iterator>::value_type, ValueType>;

	auto ceilDiv(u32 x, u32 y) -> u32 {
		return x / y + (x % y != 0);
	}

	auto floorDiv(i32 x, u32 y) -> i32 {
		return x >= 0 ? x / (i32)y : x / (i32)y - (x % y != 0);
	}

	auto outer(f32 x) -> i32 {
		return x > 0 ? (i32)ceil(x) : (i32)floor(x);
	}

	template<std::integral T>
	auto positiveMod(T a, T b) -> T {
		return (a % b + b) % b;
	}

	auto positiveMod(f32 a, f32 b) -> f32 {
		return fmod((fmod(a, b) + b), b);
	}

	auto printGrid(std::vector<i32> & grid, i32 width) -> void {
		auto height = grid.size() / width;

		for (auto y = 0; y < height; ++y) {
			for (auto x = 0; x < width; ++x) {
				std::cout << std::setfill('0') << std::setw(2) << grid[y * width + x] << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	template <typename T, Function<bool, T &> Predicate>
	auto find(std::vector<T> & vector, Predicate predicate) {
		return std::find_if(vector.begin(), vector.end(), predicate);
	}

	template <typename T, IsIterator<T> Iter>
	auto quickErase(std::vector<T> & vector, Iter iterator) -> void {
		std::iter_swap(vector.end() - 1, iterator);
		vector.pop_back();
	}

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
	auto base64Decode(const T & in) -> std::string {
		std::string out;

		std::vector<int> t(256,-1);
		for (int i = 0; i < 64; i++) t[BASE64_STRING[i]] = i;

		int val = 0, valb = -8;
		for (u8 current : in) {
			if (t[current] == -1) break;
			val = (val << 6) + t[current];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}

		return out;
	}

	template<typename T>
	constexpr auto asMutable(const T & value) noexcept -> T & {
		return const_cast<T &>(value);
	}

	template<typename T>
	constexpr auto asConst(T * pointer) noexcept -> const T * {
		return pointer;
	}
}

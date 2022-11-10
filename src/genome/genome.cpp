//
// Created by Emmet on 11/9/2022.
//

#include "genome.h"
#include "util.h"

char Genome::baseName[4] = { 'A', 'B', 'C', 'D' };

Genome::Genome(std::string & string) : code(), length((i32)string.length()) {
	code.reserve(Util::ceilDiv(length, 4_u32));

	auto constructingByte = 0_u8;
	auto baseIndex = 0_i32;

	for (auto &base : string) {
		auto part = base == 'A' ? 0
			: base == 'B' ? 1
			: base == 'C' ? 2
			: base == 'D' ? 3
			: -1;
		if (part == -1) throw std::exception("bad character in genome");

		constructingByte |= part << (3_i32 - baseIndex) * 2;
		if (++baseIndex == 4) {
			code.push_back(constructingByte);
			constructingByte = 0_u8;
			baseIndex = 0;
		}
	}

	if (baseIndex != 0) code.push_back(constructingByte);
}

auto Genome::get(i32 index) -> i32 {
	auto byte = code[index / 4_i32];
	return byte >> ((3_i32 - (index % 4_i32)) * 2_i32) & 0x3_u8;
}

auto Genome::size() -> i32 {
	return length;
}

auto Genome::toString() -> std::string {
	auto string = std::string();
	string.reserve(length);

	for (auto i = 0; i < length; ++i) {
		auto base = get(i);
		string.push_back(baseName[base]);
	}

	return string;
}

//
// Created by Emmet on 11/9/2022.
//

#include "genome.h"
#include "util.h"

char Genome::baseName[4] = { 'A', 'B', 'C', 'D' };

/**
 * default constructor initialized with empty code
 */
Genome::Genome(): code(), length(0) {}

/**
 * move constructor with externally built data
 */
Genome::Genome(std::vector<u8> && code, i32 length): code(std::move(code)), length(length) {}

/**
 * constructs a genome based on its string representation of a sequence of 'A', 'B', 'C', and 'D'
 * throws if a bad string is passed
 */
Genome::Genome(std::string & string) : code(), length((i32)string.length()) {
	code.reserve(Util::ceilDiv(length, 4_u32));

	//TODO should probably be replaced by the write() function

	/* keep a buffer of 4 bases that are written to code all at once */
	auto constructingByte = 0_u8;
	auto baseIndex = 0_i32;

	for (auto &base : string) {
		auto part = base == 'A' ? 0
			: base == 'B' ? 1
			: base == 'C' ? 2
			: base == 'D' ? 3
			: -1;
		if (part == -1) throw std::exception("bad character in genome");

		/* write to the buffer until it is full, then reset */
		constructingByte |= part << (3_i32 - baseIndex) * 2;
		if (++baseIndex == 4) {
			code.push_back(constructingByte);
			constructingByte = 0_u8;
			baseIndex = 0;
		}
	}

	/* write to code any remaining bases if the buffer is not clear */
	if (baseIndex != 0) code.push_back(constructingByte);
}

/**
 * accesses a base at an index in the code
 */
auto Genome::get(i32 index) const -> i32 {
	/*
	 * bases are packed in big endian order
	 *
	 * byte:  00 00 00 00
	 * index: 0  1  2  3
	 */
	auto byte = code[index / 4_i32];
	return byte >> ((3_i32 - (index % 4_i32)) * 2_i32) & 0x3_u8;
}

auto Genome::size() const -> i32 {
	return length;
}

/**
 * converts internal representation to string representation of ABCD
 * for displaying
 */
auto Genome::toString() const -> std::string {
	auto string = std::string();
	string.reserve(length);

	for (auto i = 0; i < length; ++i) {
		auto base = get(i);
		string.push_back(baseName[base]);
	}

	return string;
}

/**
 * appends a base to the end of the genome
 * used when constructing genome
 */
auto Genome::write(i32 base) -> void {
	/* insert in first index into a new byte */
	if (length % 4 == 0) {
		code.push_back(base << 6);

	/* insert within the existing trailing byte */
	} else {
		code[Util::ceilDiv(length, 4) - 1] |= base << ((3 - (length % 4)) * 2);
	}
	++length;
}
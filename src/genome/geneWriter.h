//
// Created by Emmet on 4/25/2023.
//

#ifndef ALIFE_GENEWRITER_H
#define ALIFE_GENEWRITER_H

#include <array>
#include "types.h"
#include "genome.h"
#include "gene.h"

/**
* these tables have been generated by an external program
*/
namespace GeneWriter {
	template<typename T>
	concept GenomeLike = requires(T t, i32 index) {
		{t[index]} -> std::same_as<Genome::Base>;
	};

	constexpr i32 decode2Table[] {
		1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0
	};

	constexpr i32 decode3Table[] {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 1, 2, 1, 1, 0, 0, 2, 1, 0, 2, 2, 1, 0, 0, 2, 2, 0, 2, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 1, 2, 2, 2, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2, 0, 2
	};

	constexpr i32 decode4Table[] {
		3, 0, 2, 3, 3, 2, 2, 3, 0, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 3, 3, 2, 2, 3, 0, 1, 2, 1, 1, 1, 1, 1, 3, 0, 3, 3, 3, 1, 1, 3, 3, 1, 1, 1, 3, 1, 1, 1, 0, 0, 2, 2, 0, 2, 2, 2, 0, 1, 0, 2, 0, 1, 0, 2
	};

	constexpr i32 decode5Table[] {
		2, 2, 2, 2, 2, 4, 4, 2, 2, 1, 1, 2, 3, 3, 4, 3, 1, 1, 1, 3, 2, 3, 4, 3, 2, 1, 1, 3, 3, 3, 4, 3, 0, 1, 1, 0, 3, 4, 4, 3, 2, 1, 1, 0, 3, 1, 1, 0, 0, 0, 4, 0, 2, 4, 4, 4, 2, 0, 0, 0, 0, 0, 4, 0
	};

	constexpr i32 decode6Table[] {
		5, 5, 2, 5, 4, 4, 4, 4, 5, 5, 2, 5, 1, 5, 2, 5, 3, 3, 2, 3, 0, 0, 0, 0, 5, 3, 2, 0, 3, 1, 2, 0, 5, 3, 2, 3, 1, 1, 0, 1, 1, 3, 2, 1, 1, 1, 2, 1, 3, 3, 2, 3, 4, 4, 0, 0, 4, 4, 2, 0, 4, 1, 4, 0
	};

	constexpr i32 decode7Table[] {
		0, 2, 2, 2, 0, 0, 2, 0, 4, 4, 2, 5, 0, 1, 2, 6, 0, 4, 2, 5, 3, 3, 3, 5, 5, 5, 5, 5, 1, 1, 1, 5, 6, 4, 4, 4, 6, 4, 3, 6, 6, 4, 5, 6, 1, 1, 1, 6, 0, 2, 3, 2, 0, 3, 3, 0, 4, 3, 3, 6, 0, 1, 1, 6
	};

	constexpr i32 decode8Table[] {
		5, 4, 7, 7, 5, 4, 7, 7, 5, 1, 1, 7, 0, 3, 7, 7, 1, 5, 1, 2, 4, 4, 4, 4, 5, 5, 1, 2, 0, 3, 3, 3, 0, 2, 1, 2, 4, 6, 6, 4, 0, 5, 1, 2, 0, 2, 1, 2, 6, 3, 6, 2, 6, 6, 6, 6, 0, 5, 0, 7, 0, 3, 3, 3
	};

	template<size_t VALUES, size_t SIZE>
	constexpr auto generateDecodedTable(const i32 (&encodedTable)[SIZE]) -> std::array<i32, VALUES> {
		auto decodedTable = std::array<i32, VALUES>();
		for (auto value = 0; value < VALUES; ++value) {
			for (auto i = 0; i < SIZE; ++i) {
				if (encodedTable[i] == value) {
					decodedTable[value] = i;
					break;
				}
			}
		}
		return decodedTable;
	}

	constexpr auto encode2Table = generateDecodedTable<2>(decode2Table);

	constexpr auto encode3Table = generateDecodedTable<3>(decode3Table);

	constexpr auto encode4Table = generateDecodedTable<4>(decode4Table);

	constexpr auto encode5Table = generateDecodedTable<5>(decode5Table);

	constexpr auto encode6Table = generateDecodedTable<6>(decode6Table);

	constexpr auto encode7Table = generateDecodedTable<7>(decode7Table);

	constexpr auto encode8Table = generateDecodedTable<8>(decode8Table);

	auto getBase(i32 sequence, i32 bases, i32 index) -> Genome::Base;

	auto write2(Genome & genome, i32 value) -> void;
	auto write3(Genome & genome, i32 value) -> void;
	auto write4(Genome & genome, i32 value) -> void;
	auto write5(Genome & genome, i32 value) -> void;
	auto write6(Genome & genome, i32 value) -> void;
	auto write7(Genome & genome, i32 value) -> void;
	auto write8(Genome & genome, i32 value) -> void;

	template<GenomeLike G>
	auto read2(G & gene, i32 offset) -> i32 {
		return decode2Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}

	template<GenomeLike G>
	auto read3(G & gene, i32 offset) -> i32 {
		return decode3Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}

	template<GenomeLike G>
	auto read4(G & gene, i32 offset) -> i32 {
		return decode4Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}

	template<GenomeLike G>
	auto read5(G & gene, i32 offset) -> i32 {
		return decode5Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}

	template<GenomeLike G>
	auto read6(G & gene, i32 offset) -> i32 {
		return decode6Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}

	template<GenomeLike G>
	auto read7(G & gene, i32 offset) -> i32 {
		return decode7Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}

	template<GenomeLike G>
	auto read8(G & gene, i32 offset) -> i32 {
		return decode8Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
	}
}

#endif //ALIFE_GENEWRITER_H

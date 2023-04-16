//
// Created by Rosa on 1/9/2023.
//

#include "gene.h"

auto Gene::write(Genome & genome) -> void {
	genome.write(headerBase());
	genome.write(headerBase());

	writeBody(genome);
}

/**
 * these tables have been generated by an external program
 */

i32 decode6Table[] {
	5, 5, 2, 5, 4, 4, 4, 4, 5, 5, 2, 5, 1, 5, 2, 5, 3, 3, 2, 3, 0, 0, 0, 0, 5, 3, 2, 0, 3, 1, 2, 0, 5, 3, 2, 3, 1, 1, 0, 1, 1, 3, 2, 1, 1, 1, 2, 1, 3, 3, 2, 3, 4, 4, 0, 0, 4, 4, 2, 0, 4, 1, 4, 0
};

i32 decode5Table[] {
	2, 2, 2, 2, 2, 4, 4, 2, 2, 1, 1, 2, 3, 3, 4, 3, 1, 1, 1, 3, 2, 3, 4, 3, 2, 1, 1, 3, 3, 3, 4, 3, 0, 1, 1, 0, 3, 4, 4, 3, 2, 1, 1, 0, 3, 1, 1, 0, 0, 0, 4, 0, 2, 4, 4, 4, 2, 0, 0, 0, 0, 0, 4, 0
};

i32 decode4Table[] {
	3, 0, 2, 3, 3, 2, 2, 3, 0, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 3, 3, 2, 2, 3, 0, 1, 2, 1, 1, 1, 1, 1, 3, 0, 3, 3, 3, 1, 1, 3, 3, 1, 1, 1, 3, 1, 1, 1, 0, 0, 2, 2, 0, 2, 2, 2, 0, 1, 0, 2, 0, 1, 0, 2
};

i32 decode3Table[] {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 1, 2, 1, 1, 0, 0, 2, 1, 0, 2, 2, 1, 0, 0, 2, 2, 0, 2, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 1, 2, 2, 2, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2, 0, 2
};

i32 decode2Table[] {
	1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0
};

inline auto generateDecodedTable(const i32 encodedTable[], i32 size, i32 values) -> std::vector<i32> {
	auto decodedTable = std::vector<i32>(values);
	for (auto value = 0; value < values; ++value) {
		for (auto i = 0; i < size; ++i) {
			if (encodedTable[i] == value) {
				decodedTable[value] = i;
				break;
			}
		}
	}
	return decodedTable;
}

auto encode6Table = generateDecodedTable(decode6Table, 64, 6);

auto encode5Table = generateDecodedTable(decode5Table, 64, 5);

auto encode4Table = generateDecodedTable(decode4Table, 64, 4);

auto encode3Table = generateDecodedTable(decode3Table, 64, 3);

auto encode2Table = generateDecodedTable(decode2Table, 64, 2);

inline auto getBase(i32 sequence, i32 bases, i32 index) -> Genome::Base {
	return (Genome::Base)((sequence >> ((bases - index - 1) * 2)) & 3);
}

/* writers */

auto Gene::write2(Genome & genome, i32 value) -> void {
	auto sequence = encode2Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto Gene::write3(Genome & genome, i32 value) -> void {
	auto sequence = encode3Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto Gene::write4(Genome & genome, i32 value) -> void {
	auto sequence = encode4Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto Gene::write5(Genome & genome, i32 value) -> void {
	auto sequence = encode5Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto Gene::write6(Genome & genome, i32 value) -> void {
	auto sequence = encode6Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

/* readers */

auto Gene::read2(GenomeView & gene, i32 offset) -> i32 {
	return decode2Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}

auto Gene::read3(GenomeView & gene, i32 offset) -> i32 {
	return decode3Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}

auto Gene::read4(GenomeView & gene, i32 offset) -> i32 {
	return decode4Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}

auto Gene::read5(GenomeView & gene, i32 offset) -> i32 {
	return decode5Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}

auto Gene::read6(GenomeView & gene, i32 offset) -> i32 {
	return decode6Table[(gene[offset] << 4) | (gene[offset + 1] << 2) | gene[offset + 2]];
}

//
// Created by Emmet on 4/25/2023.
//
#include "geneWriter.h"

inline auto getBase(i32 sequence, i32 bases, i32 index) -> Genome::Base {
	return (Genome::Base)((sequence >> ((bases - index - 1) * 2)) & 3);
}

auto GeneWriter::write2(Genome & genome, i32 value) -> void {
	auto sequence = encode2Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto GeneWriter::write3(Genome & genome, i32 value) -> void {
	auto sequence = encode3Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto GeneWriter::write4(Genome & genome, i32 value) -> void {
	auto sequence = encode4Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto GeneWriter::write5(Genome & genome, i32 value) -> void {
	auto sequence = encode5Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto GeneWriter::write6(Genome & genome, i32 value) -> void {
	auto sequence = encode6Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto GeneWriter::write7(Genome & genome, i32 value) -> void {
	auto sequence = encode7Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}

auto GeneWriter::write8(Genome & genome, i32 value) -> void {
	auto sequence = encode8Table[value];
	genome.write(getBase(sequence, 3, 0));
	genome.write(getBase(sequence, 3, 1));
	genome.write(getBase(sequence, 3, 2));
}
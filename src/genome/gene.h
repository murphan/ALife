//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_GENE_H
#define ALIFE_GENE_H

#include "genomeView.h"
#include "genome.h"

class Gene {
public:
	virtual auto headerBase() -> Genome::Base = 0;

	virtual auto writeBody(Genome & genome) -> void = 0;

	auto write(Genome & genome) -> void;

	/* reader/writer utility */

	static auto write2(Genome & genome, i32 value) -> void;
	static auto write3(Genome & genome, i32 value) -> void;
	static auto write4(Genome & genome, i32 value) -> void;
	static auto write5(Genome & genome, i32 value) -> void;
	static auto write16(Genome & genome, i32 value) -> void;

	static auto read2(GenomeView & genome, i32 offset) -> i32;
	static auto read3(GenomeView & genome, i32 offset) -> i32;
	static auto read4(GenomeView & genome, i32 offset) -> i32;
	static auto read5(GenomeView & genome, i32 offset) -> i32;
	static auto read16(GenomeView & genome, i32 offset) -> i32;
};

#endif //ALIFE_GENE_H

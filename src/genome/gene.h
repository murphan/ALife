//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_GENE_H
#define ALIFE_GENE_H

#include "genomeView.h"
#include "genome.h"

class Gene {
protected:
	/**
	 * each type of gene should implement this
	 * writes everything that's not the gene header
	 * do not call this directly, instead use write();
	 */
	virtual auto writeBody(Genome & genome) -> void = 0;

public:
	virtual auto headerBase() -> Genome::Base = 0;

	auto write(Genome & genome) -> void;

	/* reader/writer utility */

	static auto write2(Genome & genome, i32 value) -> void;
	static auto write3(Genome & genome, i32 value) -> void;
	static auto write4(Genome & genome, i32 value) -> void;
	static auto write5(Genome & genome, i32 value) -> void;
	static auto write6(Genome & genome, i32 value) -> void;
	static auto write7(Genome & genome, i32 value) -> void;

	static auto read2(GenomeView & genome, i32 offset) -> i32;
	static auto read3(GenomeView & genome, i32 offset) -> i32;
	static auto read4(GenomeView & genome, i32 offset) -> i32;
	static auto read5(GenomeView & genome, i32 offset) -> i32;
	static auto read6(GenomeView & genome, i32 offset) -> i32;
	static auto read7(GenomeView & genome, i32 offset) -> i32;
};

#endif //ALIFE_GENE_H

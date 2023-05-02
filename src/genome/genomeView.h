//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_GENOMEVIEW_H
#define ALIFE_GENOMEVIEW_H

#include "genome.h"

/**
 * accesses a section of a genome without allocating any new memory
 * backed by a pointer to the original genome
 * all accesses are treated as if they start from offset
 */
class GenomeView {
private:
	const Genome * internal;
	i32 offset;

public:
	GenomeView();
	GenomeView(const Genome * genome, i32 offset, i32 length);

	i32 length;

	auto empty() -> bool;
	auto operator[](i32 index) const -> Genome::Base;
};

#endif //ALIFE_GENOMEVIEW_H

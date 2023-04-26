//
// Created by Rosa on 1/9/2023.
//

#ifndef ALIFE_GENE_H
#define ALIFE_GENE_H

#include <array>
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
	enum Type {
		BODY,
		EYE,
		UPGRADE,
		MUTATION,
	};
};

#endif //ALIFE_GENE_H

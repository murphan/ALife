//
// Created by Emmet on 3/5/2023.
//

#ifndef ALIFE_INITIALGENOME_H
#define ALIFE_INITIALGENOME_H

#include "genome/genome.h"

class InitialGenome {
public:
	static auto create() -> Genome;
};

#endif //ALIFE_INITIALGENOME_H

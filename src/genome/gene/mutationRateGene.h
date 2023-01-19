//
// Created by Rosa on 1/12/2023.
//

#ifndef ALIFE_MUTATIONRATEGENE_H
#define ALIFE_MUTATIONRATEGENE_H

#include <initializer_list>
#include "../gene.h"

class MutationRateGene : public Gene {
protected:
	auto writeBody(Genome & genome) -> void override;

public:
	enum MutationType {
		SUBSTITUTION = 0,
		INSERTION = 1,
		DELETION = 2,
		NONE = 3,
	};

private:
	MutationType types[4];
	/* 0 for down, 1 for up */
	i32 changes[4];

public:
	explicit MutationRateGene(GenomeView & view);

	struct Entry { MutationType type; i32 change; };
	MutationRateGene(std::initializer_list<Entry> && list);

	auto headerBase() -> Genome::Base override;

	struct Results { i32 substitution; i32 insertion; i32 deletion; };
	auto getResults() -> Results;
};

#endif //ALIFE_MUTATIONRATEGENE_H

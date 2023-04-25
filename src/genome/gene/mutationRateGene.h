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

private:
	i32 change;

public:
	static const i32 LENGTH;

	explicit MutationRateGene(GenomeView & view);

	explicit MutationRateGene(i32 change);

	auto headerBase() -> Genome::Base override;

	auto getChange() -> i32;
};

#endif //ALIFE_MUTATIONRATEGENE_H

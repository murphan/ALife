//
// Created by Emmet on 4/26/2023.
//

#include "../gene.h"

#ifndef ALIFE_MOVEMENTGENE_H
#define ALIFE_MOVEMENTGENE_H

class MovementGene : public Gene {
protected:
	auto writeBody(Genome & genome) -> void override;

private:
	i32 change;

public:
	static const i32 LENGTH;

	explicit MovementGene(GenomeView & view);

	explicit MovementGene(i32 change);

	auto getChange() const -> i32;
};

#endif //ALIFE_MOVEMENTGENE_H

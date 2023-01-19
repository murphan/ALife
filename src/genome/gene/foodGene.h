//
// Created by Rosa on 1/12/2023.
//

#ifndef ALIFE_FOODGENE_H
#define ALIFE_FOODGENE_H

#include "../gene.h"
#include "../../food.h"

/**
 * organims with this gene get more from the food they eat
 * each gene only affects one food type
 *
 * it has the potential to give two bonuses, one for digestion,
 * and for for absoprtion (not yet decided what these mean(
 *
 * the two bonuses could be the same, or different
 *
 * use the digestionBonus() and absorptionBonus() to get the final
 * resulting bonuses to either stat from this gene
 */
class FoodGene : public Gene {
public:
	enum Bonus {
		DIGESTION = 0,
		ABSORPTION = 1,
	};

private:
	Bonus bonus0;
	Bonus bonus1;

protected:
	auto writeBody(Genome & genome) -> void override;

public:
	FoodType foodType;

	explicit FoodGene(GenomeView & view);
	FoodGene(FoodType foodType, Bonus bonus0, Bonus bonus1);

	auto headerBase() -> Genome::Base override;

	/* getters */

	auto digestionBonus() -> i32;
	auto absorptionBonus() -> i32;
};

#endif //ALIFE_FOODGENE_H

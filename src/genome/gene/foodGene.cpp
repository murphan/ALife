//
// Created by Rosa on 1/12/2023.
//

#include "foodGene.h"

FoodGene::FoodGene(GenomeView & view) :
	foodType((FoodType) read4(view, 0)),
	bonus0((Bonus) read2(view, 2)),
	bonus1((Bonus) read2(view, 4)) {}

FoodGene::FoodGene(FoodType foodType, Bonus bonus0, Bonus bonus1) :
	foodType(foodType),
	bonus0(bonus0),
	bonus1(bonus1) {}

auto FoodGene::headerBase() -> Genome::Base {
	return Genome::C;
}

auto FoodGene::writeBody(Genome & genome) -> void {
	write4(genome, foodType);
	write2(genome, bonus0);
	write2(genome, bonus1);
}

auto FoodGene::digestionBonus() -> i32 {
	return bonus0 == Bonus::DIGESTION + bonus1 == Bonus::DIGESTION;
}

auto FoodGene::absorptionBonus() -> i32 {
	return bonus0 != Bonus::DIGESTION + bonus1 != Bonus::DIGESTION;
}

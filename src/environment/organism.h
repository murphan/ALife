//
// Created by Emmet on 2/3/2023.
//

#ifndef ALIFE_ORGANISM_H
#define ALIFE_ORGANISM_H

#include "types.h"

#include "genome/genome.h"
#include "genome/phenome.h"
#include "genome/body.h"

class Organism {
private:
	Phenome phenome;

public:
	i32 x, y;
	i32 rotation;

	i32 energy;
	i32 age;

	Organism(Phenome && phenome, i32 x, i32 y);

	auto getPhenome() const -> const Phenome &;
	auto getGenome() const -> const Genome &;
	auto body() const -> const Body &;

	auto tick() -> i32;
};

#endif //ALIFE_ORGANISM_H

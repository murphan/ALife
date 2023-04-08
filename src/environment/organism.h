//
// Created by Emmet on 2/3/2023.
//

#ifndef ALIFE_ORGANISM_H
#define ALIFE_ORGANISM_H

#include "types.h"

#include "genome/genome.h"
#include "genome/phenome.h"
#include "genome/body.h"
#include "environment/food.h"
#include "uuid.h"
#include "json.hpp"

class Organism {
private:
	using json = nlohmann::json;

	Phenome phenome;

public:
	UUID uuid;

	i32 x, y;
	Direction rotation;

	i32 energy;
	i32 age;

	Organism(Organism && other) = default;
	Organism(Phenome && phenome, UUID uuid, i32 x, i32 y, Direction rotation, i32 energy);

	auto operator=(Organism && other) noexcept -> Organism & = default;

	auto getPhenome() const -> const Phenome &;
	auto getGenome() const -> const Genome &;
	auto body() const -> const Body &;

	auto tick() -> i32;

	auto serialize(bool detailed) const -> json;
};

#endif //ALIFE_ORGANISM_H

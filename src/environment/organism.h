//
// Created by Emmet on 2/3/2023.
//

#ifndef ALIFE_ORGANISM_H
#define ALIFE_ORGANISM_H

#include "types.h"

#include "genome/genome.h"
#include "genome/phenome.h"
#include "genome/body.h"
#include "uuid.h"
#include "json.hpp"

class Organism {
private:
	using json = nlohmann::json;

public:
	Phenome phenome;

	u32 id;

	i32 x, y;
	Direction rotation;

	i32 energy;
	i32 age;

	i32 ticksSinceCollision;
	Direction movementDirection;

	std::optional<Phenome> storedChild;

	Organism(Organism && other) = default;
	Organism(Phenome && phenome, u32 id, i32 x, i32 y, Direction rotation, i32 energy, Direction movementDirection);

	auto operator=(Organism && other) noexcept -> Organism & = default;

	auto getGenome() const -> const Genome &;
	auto body() -> Body &;

	auto serialize(bool detailed) -> json;
};

#endif //ALIFE_ORGANISM_H

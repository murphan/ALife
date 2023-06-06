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
#include "tree.h"

class Organism {
private:
	using json = nlohmann::json;

public:
	Phenome phenome;

	u32 id;

	i32 x, y;
	Direction rotation;

	i32 energy;

	i32 ticksSinceCollision;
	i32 ticksStuck;

	std::optional<Phenome> storedChild;
	Tree::Node * node;

	Organism(Organism && other) = default;
	Organism(Phenome && phenome, u32 id, i32 x, i32 y, Direction rotation, i32 energy, Tree::Node * node);

	auto operator=(Organism && other) noexcept -> Organism & = default;

	auto getGenome() const -> const Genome &;
	auto body() -> Body &;

	auto serialize(bool detailed) -> json;

	auto addEnergy(i32 delta, Settings & settings) -> void;

	auto absoluteXY(Body::Cell & cell) const -> Util::Coord;
};

#endif //ALIFE_ORGANISM_H

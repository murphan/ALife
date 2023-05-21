//
// Created by Emmet on 2/3/2023.
//

module;

#include <optional>

export module Organism;

import Rotation;
import Node;
import Body;
import Phenome;
import Types;
import Direction;
import Util;
import Genome;
import Cell;
import Coord;

export class Organism {
public:
	Phenome phenome;

	u32 id;

	i32 x, y;
	Direction rotation;

	i32 energy;

	i32 ticksSinceCollision;
	i32 ticksStuck;

	std::optional<Phenome> storedChild;
	Node * node;

	Organism(Organism && other) = default;
	Organism(Phenome && phenome, u32 id, i32 x, i32 y, Direction rotation, i32 energy, Node * node) :
		id(id),
		phenome(std::move(phenome)),
		x(x),
		y(y),
		rotation(rotation),
		energy(energy),
		storedChild(std::nullopt),
		ticksSinceCollision(0),
		ticksStuck(0),
		node(node) {}

	auto operator=(Organism && other) noexcept -> Organism & = default;

	[[nodiscard]] auto getGenome() const -> const Genome & {
		return phenome.genome;
	}

	[[nodiscard]] auto body() const -> const Body & {
		return phenome.body;
	}

	auto body() -> Body & {
		return Util::asMutable(Util::asConst(this)->body());
	}

	auto addEnergy(i32 delta) -> void {
		energy += delta;
		if (energy < 0) energy = 0;
	};

	[[nodiscard]] auto absoluteXY(Cell cell) const -> Coord {
		return Body::absoluteXY(cell, x, y, rotation);
	};
};

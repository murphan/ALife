//
// Created by Emmet on 2/3/2023.
//

#include "organism.h"
#include "util/rotation.h"

Organism::Organism(Phenome && phenome, u32 id, i32 x, i32 y, Direction rotation, i32 energy, Tree::Node * node) :
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

auto Organism::getGenome() const -> const Genome & {
	return phenome.genome;
}

auto Organism::body() -> Body & {
	return phenome.body;
}

auto Organism::serialize(bool detailed) -> json {
	auto && body = phenome.body;

	auto nonDetailedPart = json {
		{ "id", id },
		{ "rotation", rotation.value() },
		{ "x", x },
		{ "y", y },
		{ "energy", energy },
		{ "numAliveCells", phenome.numAliveCells },
		{ "baseBodyEnergy", phenome.baseBodyEnergy },
	};

	if (!detailed) return nonDetailedPart;

	nonDetailedPart.push_back({ "genome", phenome.genome.toString() });

	auto cells = json::array();
	for (auto && cell : body.getCells()) {
		cells.push_back({
            { "x", cell.x() },
            { "y", cell.y() },
            { "type", cell.bodyPart() }
        });
	}
	nonDetailedPart.push_back({ "cells", cells });

	auto mutationModifiers = json::array();
	for (auto && mutationModifier : phenome.mutationModifiers) mutationModifiers.push_back(mutationModifier);
	nonDetailedPart.push_back({ "mutationModifiers", mutationModifiers });

	nonDetailedPart.push_back({ "moveTries", phenome.moveTries });

	nonDetailedPart.push_back({ "moveLength", phenome.moveLength });

	auto reactions = json::array();
	for (auto && reaction : phenome.reactions) reactions.push_back(json {
		{ "seeing", BODY_PART_NAMES[reaction.seeing] },
		{ "action", EyeGene::ACTION_NAMES[reaction.actionType] },
	});
	nonDetailedPart.push_back({ "reactions", reactions });

	auto && completeParts = nonDetailedPart;
	return completeParts;
}

/**
 * do not let energy go negative
 */
auto Organism::addEnergy(i32 delta, Settings & settings) -> void {
	energy += delta;
	if (energy < 0) energy = 0;
}

auto Organism::absoluteXY(Body::Cell & cell) const -> Util::Coord {
	return Body::absoluteXY(cell, x, y, rotation);
}

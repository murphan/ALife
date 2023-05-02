//
// Created by Emmet on 2/3/2023.
//

#include "organism.h"
#include "genome/rotation.h"

Organism::Organism(Phenome && phenome, u32 id, i32 x, i32 y, Direction rotation, i32 energy) :
	id(id),
	phenome(std::move(phenome)),
	x(x),
	y(y),
	rotation(rotation),
	energy(energy),
	storedChild(std::nullopt),
	ticksSinceCollision(0),
	ticksStuck(0) {}

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
		{ "left", body.getLeft(rotation) },
		{ "right", body.getRight(rotation) },
		{ "down", body.getDown(rotation) },
		{ "up", body.getUp(rotation) },
		{ "rotation", rotation.value() },
		{ "x", x },
		{ "y", y },
		{ "energy", energy },
	};

	if (!detailed) return nonDetailedPart;

	auto mutationModifiers = json::array();
	for (auto && mutationModifier : phenome.mutationModifiers) mutationModifiers.push_back(mutationModifier);

	auto eyeReactions = json::array();
	for (auto && eyeReaction : phenome.eyeReactions) eyeReactions.push_back(json {
		{ "seeing", eyeReaction.seeing },
		{ "action", EyeGene::ACTION_NAMES[eyeReaction.actionType] },
	});

	auto cells = json::array();
	for (auto && cell : body.getCells()) {
		cells.push_back({
            { "x", cell.x() },
            { "y", cell.y() },
            { "type", cell.bodyPart() }
		});
	}

	nonDetailedPart.push_back({ "mutationModifiers", mutationModifiers });
	nonDetailedPart.push_back({ "eyeReactions", eyeReactions });
	nonDetailedPart.push_back({ "genome", phenome.genome.toString() });
	nonDetailedPart.push_back({ "cells", cells });
	auto && completeParts = nonDetailedPart;
	return completeParts;
}

/**
 * do not let energy go negative
 */
auto Organism::addEnergy(i32 delta) -> void {
	energy += delta;
	if (energy < 0) energy = 0;
}

auto Organism::absoluteXY(Body::Cell & cell) const -> Util::Coord {
	return Body::absoluteXY(cell, x, y, rotation);
}

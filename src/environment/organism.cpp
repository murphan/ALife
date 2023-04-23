//
// Created by Emmet on 2/3/2023.
//

#include "organism.h"

Organism::Organism(Phenome && phenome, UUID uuid, i32 x, i32 y, Direction rotation, i32 energy, Direction movementDirection) :
	uuid(uuid),
	phenome(std::move(phenome)),
	x(x),
	y(y),
	rotation(rotation),
	energy(energy),
	storedChild(std::nullopt),
	age(0),
	movementDirection(movementDirection),
	movementSearching(true),
	ticksSinceCollision(0) {}

auto Organism::getPhenome() const -> const Phenome & {
	return phenome;
}

auto Organism::getGenome() const -> const Genome & {
	return phenome.genome;
}

auto Organism::body() const -> const Body & {
	return phenome.body;
}

auto Organism::serialize(bool detailed) const -> json {
	auto && body = phenome.body;

	auto nonDetailedPart = json {
		{ "id", uuid.asString() },
		{ "left", body.getLeft(rotation) },
		{ "right", body.getRight(rotation) },
		{ "down", body.getDown(rotation) },
		{ "up", body.getUp(rotation) },
		{ "rotation", rotation.value() },
		{ "x", x },
		{ "y", y },
		{ "energy", energy },
		{ "age", age },
	};

	if (!detailed) return nonDetailedPart;

	auto mutationModifiers = json::array();
	for (auto && mutationModifier : phenome.mutationModifiers) mutationModifiers.push_back(mutationModifier);

	auto foodStats = json::array();
	for (auto && foodStat : phenome.foodStats) foodStats.push_back(json {
		{ "digestionBonus", foodStat.digestionBonus },
		{ "absorptionBonus", foodStat.absoprtionBonus },
	});

	auto eyeReactions = json::array();
	for (auto && eyeReaction : phenome.eyeReactions) eyeReactions.push_back(json {
		{ "seeingThing", EyeGene::SEEING_THING_NAMES[eyeReaction.seeingThing] },
		{ "specific", eyeReaction.specific },
		{ "foodType", eyeReaction.getFoodType() },
		{ "friendly", eyeReaction.getFriendly() },
	});
	auto environmentReactions = json::array();
	for (auto && environmentReaction : phenome.environmentReactions) environmentReactions.push_back(json {
		{ "factor", environmentReaction.factor },
		{ "above", environmentReaction.getAbove() },
		{ "threshold", environmentReaction.getThreshold() },
	});

	auto byteEncodedBody = std::string();
	byteEncodedBody.reserve(body.getWidth() * body.getHeight());
	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			byteEncodedBody.push_back((char)body.access(i, j, rotation).bodyPart());
		}
	}

	nonDetailedPart.push_back({ "mutationModifiers", mutationModifiers });
	nonDetailedPart.push_back({ "foodStats", foodStats });
	nonDetailedPart.push_back({ "eyeReactions", eyeReactions });
	nonDetailedPart.push_back({ "environmentReactions", environmentReactions });
	nonDetailedPart.push_back({ "genome", phenome.genome.toString() });
	nonDetailedPart.push_back({ "body", Util::base64Encode(byteEncodedBody) });
	auto && completeParts = nonDetailedPart;
	return completeParts;
}

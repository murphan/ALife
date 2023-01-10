//
// Created by Emmet on 11/10/2022.
//

#ifndef ALIFE_GENOMECREATOR_H
#define ALIFE_GENOMECREATOR_H

#include <phenome/phenome.h>
#include "food.h"
#include "types.h"
#include "direction.h"
#include "genome.h"
#include "../phenome/reaction.h"

/**
 * user friendly representation of a body instruction gene
 * used only to manually create organisms
 *
 * body instructions have 4 modes:
 * nothing,
 * duplicate,
 * use anchor,
 * set anchor
 *
 * each can be created with a static factory function
 */
class BodyInstruction {
public:
	Direction direction;
	/** 1 - 16 */
	i32 type;
	i32 anchor;
	i32 setAnchor;
	bool duplicate;

	auto usesAnchor() const -> bool;
	auto setsAnchor() const -> bool;

	static auto create(Direction, i32) -> BodyInstruction;
	static auto createDuplicate(Direction, i32) -> BodyInstruction;
	static auto createUseAnchor(Direction, i32, i32) -> BodyInstruction;
	static auto createSetAnchor(Direction, i32, i32) -> BodyInstruction;
};

/**
 * allows the the manual creation of a genome in code to build an initial organism.
 * has an internal genome which is updated immediately as the add gene functions are called.
 * call get after adding genes to copy the genome out.
 */
class GenomeCreator {
private:
	/* how many garbage bases are placed between genes */
	i32 filler;
	Genome genome;

public:
	GenomeCreator(i32 filler);

	auto addBodyGene(BodyInstruction && instruction) -> void;
	auto addEyeGene(EyeReaction && reaction) -> void;
	auto addEnvironmentGene(EnvironmentReaction && reaction) -> void;
	auto addFoodGene(FoodType type, FoodStats::Type digestion0, FoodStats::Type digestion1) -> void;
	/**
	 * @param strength0 only -2, -1, 1, or 2
	 * @param strength1 only -2, -1, 1, or 2
	 */
	auto addMutationRateGene(Phenome::MutationType type0, i32 strength0, Phenome::MutationType type1, i32 strength1) -> void;

	auto get() const -> const Genome &;
};

#endif //ALIFE_GENOMECREATOR_H

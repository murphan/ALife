//
// Created by Emmet on 2/3/2023.
//

#include "organism.h"

Organism::Organism(Phenome && phenome, UUID uuid, i32 x, i32 y) :
	uuid(uuid),
	phenome(std::move(phenome)),
	x(x),
	y(y),
	rotation(0),
	energy(0),
	age(0) {}

auto Organism::getPhenome() const -> const Phenome & {
	return phenome;
}

auto Organism::getGenome() const -> const Genome & {
	return phenome.genome;
}

auto Organism::body() const -> const Body & {
	return phenome.body;
}

/**
 * @return the organism's new age after the tick
 */
auto Organism::tick() -> i32 {
	return ++age;
}

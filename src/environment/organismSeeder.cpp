//
// Created by Emmet on 2/14/2023.
//
#include "organismSeeder.h"

auto OrganismSeeder::insertInitialOrganisms(
	std::vector<Organism> & insertList,
	const Environment & environment,
	const Phenome & initialPhenome,
	const Settings & settings,
	i32 count,
	std::default_random_engine & random
) -> void {
	insertList.reserve(count);

	auto && body = initialPhenome.body;

	auto spaceWide = body.getWidth() + 1;
	auto spaceTall = body.getHeight() + 1;

	auto offsetX = spaceWide / 2 - body.getLeft(Direction::RIGHT);
	auto offsetY = spaceTall / 2 - body.getDown(Direction::RIGHT);

	auto centerX = environment.getWidth() / 2;
	auto centerY = environment.getHeight() / 2;

	auto sideLength = (i32)ceil(sqrt((f32)count));

	/* fill in `count` spaces out of a square of side length `sideLength` */
	for (auto c = 0; c < count; ++c) {
		auto gridY = (c / sideLength) - (sideLength / 2), gridX = (c % sideLength) - (sideLength / 2);

		auto copiedPhenome = initialPhenome;
		auto initialEnergy = copiedPhenome.survivalEnergy + settings.startingEnergy;

		insertList.emplace_back(
			std::move(copiedPhenome),
			UUID::generateRandom(),
			centerX + gridX * spaceWide - offsetX,
			centerY + gridY * spaceTall - offsetY,
			Direction::RIGHT,
			initialEnergy,
			std::uniform_int_distribution(0, 7)(random)
		);
	}
}


//
// Created by Emmet on 2/14/2023.
//
#include "organismSeeder.h"

auto OrganismSeeder::insertInitialOrganisms(
	std::vector<Organism> & insertList,
	const Environment & environment,
	std::default_random_engine & random,
	Ids & ids,
	Tree & tree,
	const Phenome & initialPhenome,
	const Settings & settings,
	i32 count
) -> void {
	auto && initialGenome = initialPhenome.genome;

	insertList.reserve(count);

	auto && body = initialPhenome.body;

	auto spaceWide = body.getWidth() + 1;
	auto spaceTall = body.getHeight() + 1;

	auto offsetX = spaceWide / 2 - body.getLeft(Direction::RIGHT);
	auto offsetY = spaceTall / 2 - body.getDown(Direction::RIGHT);

	auto centerX = environment.getWidth() / 2;
	auto centerY = environment.getHeight() / 2;

	auto sideLength = (i32)ceil(sqrt((f32)count));

	if (count > 1) tree.add(nullptr, initialGenome);

	/* fill in `count` spaces out of a square of side length `sideLength` */
	for (auto c = 0; c < count; ++c) {
		auto gridY = (c / sideLength) - (sideLength / 2), gridX = (c % sideLength) - (sideLength / 2);

		auto copiedPhenome = initialPhenome;
		auto initialEnergy = settings.startingEnergy;

		insertList.emplace_back(
			std::move(copiedPhenome),
			ids.newId(random),
			centerX + gridX * spaceWide - offsetX,
			centerY + gridY * spaceTall - offsetY,
			Direction::RIGHT,
			initialEnergy,
			tree.add(tree.root.get(), initialGenome)
		);
	}
}


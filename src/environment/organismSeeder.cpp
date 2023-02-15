//
// Created by Emmet on 2/14/2023.
//
#include "organismSeeder.h"

auto OrganismSeeder::insertInitialOrganisms(
	std::vector<Organism> & insertList,
	const Environment & environment,
	const Phenome & initialPhenome,
	i32 count
) -> void {
	insertList.reserve(count);

	auto && body = initialPhenome.body;

	auto bodyWidth = body.getWidth();
	auto bodyHeight = body.getHeight();

	auto offsetX = bodyWidth / 2 - body.left;
	auto offsetY = bodyHeight / 2 - body.down;

	auto centerX = environment.getWidth() / 2;
	auto centerY = environment.getHeight() / 2;

	auto sideLength = (i32)ceil(sqrt((f32)count));

	/* fill in count of out sideLength * sideLength spaces in a square */
	for (auto c = 0; c < count; ++c) {
		auto gridY = (c / sideLength) - (sideLength / 2), gridX = (c % sideLength) - (sideLength / 2);

		auto newPhenome = initialPhenome;
		insertList.emplace_back(
			std::move(newPhenome),
			UUID::generateRandom(),
			centerX + gridX * bodyWidth - offsetX,
			centerY + gridY * bodyHeight - offsetY
		);
	}
}


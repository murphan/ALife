//
// Created by Emmet on 4/27/2023.
//

#include "mouth.h"

auto Mouth::tick(i32 x, i32 y, Organism & organism, i32 index, Environment & environment, OrganismGrid & organismGrid, Settings & settings) -> void {
	tryEat(x, y, organism, index, environment, organismGrid, settings);
	tryEat(x + 1, y, organism, index, environment, organismGrid, settings);
	tryEat(x, y + 1, organism, index, environment, organismGrid, settings);
	tryEat(x - 1, y, organism, index, environment, organismGrid, settings);
	tryEat(x, y - 1, organism, index, environment, organismGrid, settings);
}

auto Mouth::tryEat(i32 x, i32 y, Organism & organism, i32 index, Environment & environment, OrganismGrid & organismGrid, Settings & settings) -> void {
	auto && space = organismGrid.access(x, y);
	auto && envSpace = environment.access(x, y);

	/* eating broken dead cells */
	if (envSpace.food.filled()) {
		eatCell(organism, envSpace.food, settings);

		envSpace.food = Body::Cell::makeEmpty();
		space = OrganismGrid::Space::makeEmpty();
	}

	/* eating non-broken dead cells */
	else if (space.isFilled() && space.index() != index && space.cell().dead()) {
		eatCell(organism, space.cell(), settings);

		space.cell() = Body::Cell::makeEmpty();
		space = OrganismGrid::Space::makeEmpty();
	}
}

auto Mouth::eatCell(Organism & organism, Body::Cell & cell, Settings & settings) -> void {
	auto energy = (i32)((f32)cell.cost(settings) * settings.foodEfficiency);
	organism.addEnergy(energy);
}

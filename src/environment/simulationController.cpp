
#include "environment/simulationController.h"

SimulationController::SimulationController(Environment && environment) :
	random(std::random_device()()),
	environment(std::move(environment)),
	organisms(),
	currentStep(0) {}

auto SimulationController::tempId(i32 index) -> i32 {
	return index + 1;
}

auto SimulationController::step() -> void {
	std::shuffle(organisms.begin(), organisms.end(), random);

	auto organismGrid = OrganismGrid(environment.getWidth(), environment.getHeight());

	moveOrganisms(organismGrid);

	organismsAgeAndDie();

	++currentStep;
}

auto SimulationController::moveOrganisms(OrganismGrid & organismGrid) -> void {
	for (auto i = 0; i < organisms.size(); ++i) {
		organismGrid.placeOrganism(organisms.at(i), tempId(i));
	}

	for (auto i = 0; i < organisms.size(); ++i) {
		auto deltaX = std::uniform_int_distribution(-1, 1)(random);
		auto deltaY = std::uniform_int_distribution(-1, 1)(random);

		auto && organism = organisms.at(i);
		auto id = tempId(i);

		if (organismGrid.canMoveOrganism(organism, id, deltaX, deltaY)) {
			organismGrid.moveOrganism(organism, id, deltaX, deltaY);
		}
	}
}

auto SimulationController::organismsAgeAndDie() -> void {
	std::erase_if(organisms, [this](auto && organism) {
		auto newAge = organism.tick();

		//TODO put in settings
		constexpr auto AGE_FACTOR = 16;

		if (newAge > organism.getPhenome().maxAge(AGE_FACTOR)) {
			replaceOrganismWithFood(organism);
			return true;
		} else {
			return false;
		}
	});
}

auto SimulationController::replaceOrganismWithFood(const Organism & organism) -> void {
	auto && body = organism.body();

	for (auto j = body.down; j <= body.up; ++j) {
		for (auto i = body.left; i <= body.right; ++i) {
			auto y = organism.y + j, x = organism.x + i;
			auto cell = body.access(i, j);

			if (cell != BodyPart::NONE) {
				environment.getCell(x, y).setFood(Food(Food::FOOD0, 1));
			}
		}
	}
}

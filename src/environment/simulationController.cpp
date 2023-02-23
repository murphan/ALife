
#include "environment/simulationController.h"

SimulationController::SimulationController(Environment && environment) :
	random(std::random_device()()),
	organismGrid(environment.getWidth(), environment.getHeight()),
	environment(std::move(environment)),
	organisms(),
	currentStep(0) {}

auto SimulationController::tempId(i32 index) -> i32 {
	return index + 1;
}

auto SimulationController::step() -> void {
	std::shuffle(organisms.begin(), organisms.end(), random);

	moveOrganisms();

	organismsAgeAndDie();

	++currentStep;
}

auto symmetricRange = std::uniform_int_distribution<i32>(-1, 1);

auto SimulationController::moveOrganisms() -> void {
	organismGrid.clear();

	for (auto i = 0; i < organisms.size(); ++i) {
		organismGrid.placeOrganism(organisms.at(i), tempId(i));
	}

	for (auto i = 0; i < organisms.size(); ++i) {
		auto deltaX = symmetricRange(random);
		auto deltaY = symmetricRange(random);
		auto deltaRotation = symmetricRange(random);

		auto && organism = organisms.at(i);
		auto id = tempId(i);

		if (organismGrid.canMoveOrganism(organism, id, deltaX, deltaY, deltaRotation)) {
			organismGrid.moveOrganism(organism, id, deltaX, deltaY, deltaRotation);
		}
	}
}

auto SimulationController::organismsAgeAndDie() -> void {
	std::erase_if(organisms, [this](auto && organism) {
		auto newAge = organism.tick();

		//TODO put in settings
		constexpr auto AGE_FACTOR = 1600;

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
	auto rotation = organism.rotation;

	for (auto j = body.getDown(rotation); j <= body.getUp(rotation); ++j) {
		for (auto i = body.getLeft(rotation); i <= body.getRight(rotation); ++i) {
			auto y = organism.y + j, x = organism.x + i;
			auto cell = body.access(i, j, rotation);

			if (cell != BodyPart::NONE) {
				environment.getCell(x, y).setFood(Food(Food::FOOD0, 1));
			}
		}
	}
}

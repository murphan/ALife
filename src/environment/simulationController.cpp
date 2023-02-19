
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
		//organisms[0].rotation = Direction::UP;
		organismGrid.placeOrganism(organisms.at(i), tempId(i));
	}

	for (auto i = 0; i < organisms.size(); ++i) {
		auto deltaX = 0; //std::uniform_int_distribution(-1, 1)(random);
		auto deltaY = 0; //std::uniform_int_distribution(-1, 1)(random);
		auto deltaRotation = std::uniform_int_distribution(-1, 1)(random) * 2;

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

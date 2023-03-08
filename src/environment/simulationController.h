
#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "environment/environment.h"

#include <vector>

#include "controls.h"
#include "organism.h"
#include "organismGrid.h"

using json = nlohmann::json;

class SimulationController {
private:
	std::default_random_engine random;

	OrganismGrid organismGrid;

	auto tempId(i32 index) -> i32;

	/* substep functions */

	auto moveOrganisms() -> void;

	auto organismsAgeAndDie() -> void;

	auto replaceOrganismWithFood(const Organism & organism) -> void;

    auto organismsEat() -> void;


public:
	Environment environment;
	std::vector<Organism> organisms;

	int currentTick;

    explicit SimulationController(Environment && environment);

	auto tick() -> void;

	auto serialize() -> json;

    auto addFood(i32 foodX, i32 foodY, Food::Type type, i32 energy) -> void;
    auto scatterFood(Food::Type type, i32 numFood, i32 energyDefault) -> void;
    auto howMuchFood() -> i32;

	auto getOrganism(UUID & id) -> Organism *;
};

#endif //ALIFE_SIMULATIONCONTROLLER_H

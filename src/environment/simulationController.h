
#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "environment/environment.h"

#include <vector>

#include "organism.h"
#include "organismGrid.h"

class SimulationController {
private:
	std::default_random_engine random;

	OrganismGrid organismGrid;

	auto tempId(i32 index) -> i32;

	/* substep functions */

	auto moveOrganisms() -> void;

	auto organismsAgeAndDie() -> void;

	auto replaceOrganismWithFood(const Organism & organism) -> void;


public:
	Environment environment;
	std::vector<Organism> organisms;
	int currentStep;

    explicit SimulationController(Environment && environment);
    auto addFood(i32 foodX, i32 foodY, Food::Type type) -> void;
    auto tick() -> void;
};

#endif //ALIFE_SIMULATIONCONTROLLER_H


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

public:
	Environment environment;
	std::vector<Organism> organisms;

	int currentStep;

    explicit SimulationController(Environment && environment);

    auto step() -> void;

	auto serialize() -> json;
};

#endif //ALIFE_SIMULATIONCONTROLLER_H

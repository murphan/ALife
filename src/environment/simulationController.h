
#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "environment/environment.h"

#include <vector>
#include <span>

#include "controls.h"
#include "organism.h"
#include "organismGrid.h"
#include "settings.h"
#include "ids.h"

using json = nlohmann::json;

class SimulationController {
private:
	std::default_random_engine & random;
	Ids & ids;
	Settings & settings;

	/* substep functions */

	auto organismSeeingDirection(Organism & organism, i32 index) -> std::optional<Direction>;

	auto shuffleOrganisms() -> void;

	auto updateFactors() -> void;

	auto ageOrganismCells() -> void;

	auto moveOrganisms() -> void;

	auto organismCellsTick() -> void;

	auto checkOrganismsDie() -> void;

	auto organismsReproduce() -> void;

	/* helpers */

	auto updateOrganismIndices() -> void;

	auto replaceOrganismWithFood(Organism & organism) -> void;

	auto findChildSpawnPoint(Organism & organism, Phenome & childPhenome) -> std::optional<Util::Coord>;

    auto tryReproduce(Phenome & childPhenome, Organism & organism, i32 reproductionEnergy, i32 childBodyEnergy, i32 childEnergy) -> std::optional<Organism>;

public:
	OrganismGrid organismGrid;
	Environment environment;
	std::vector<Organism> organisms;

	int currentTick;

    explicit SimulationController(Environment && environment, OrganismGrid && organismGrid, std::default_random_engine & random, Ids & ids, Settings & settings);

	auto refreshFactors() -> void;
	auto tick() -> void;

	auto serialize() -> json;

	auto getOrganism(u32 id) -> Organism *;
};

#endif //ALIFE_SIMULATIONCONTROLLER_H

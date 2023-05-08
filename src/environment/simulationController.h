
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
#include "tree.h"

using json = nlohmann::json;

class SimulationController {
private:
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

	auto replaceOrganismWithFood(Organism & organism) -> void;

	struct ChildSpawnPoint {
		i32 x;
		i32 y;
		Direction rotation;
	};

	auto findChildSpawnPoint(Organism & organism, Phenome & childPhenome) -> std::optional<ChildSpawnPoint>;

    auto tryReproduce(Phenome & childPhenome, Organism & organism, i32 reproductionEnergy, i32 childBodyEnergy, i32 childEnergy) -> std::optional<Organism>;

public:
	Environment environment;
	OrganismGrid organismGrid;
	std::default_random_engine random;
	Ids ids;
	Tree tree;
	std::vector<Organism> organisms;

	int currentTick;

	auto renderOrganismGrid() -> void;

    explicit SimulationController(
	    Settings & settings,
		Environment && environment,
		OrganismGrid && organismGrid,
		std::default_random_engine random,
		Ids && ids,
		Tree && tree
	);

	auto operator=(SimulationController && other) noexcept -> SimulationController &;

	auto refreshFactors() -> void;
	auto tick() -> void;

	auto serialize() -> json;

	auto getOrganism(u32 id) -> Organism *;
};

#endif //ALIFE_SIMULATIONCONTROLLER_H

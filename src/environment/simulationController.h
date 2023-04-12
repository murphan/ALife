
#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "environment/environment.h"

#include <vector>
#include <span>

#include "controls.h"
#include "organism.h"
#include "organismGrid.h"
#include "settings.h"

using json = nlohmann::json;

class SimulationController {
private:
	std::default_random_engine random;

	OrganismGrid organismGrid;

	/* substep functions */

	auto moveOrganisms(Settings & settings) -> std::vector<i32>;

	auto doDamageAndKill(std::vector<i32> & damages, Settings & settings) -> void;

	auto organismsAgeAndDie(Settings & settings) -> void;

	auto replaceOrganismWithFood(const Organism & organism, Settings & settings) -> void;

    auto organismsEat(Settings & settings, std::vector<i32> & damages) -> void;

    auto organismsReproduce(Settings & settings) -> void;

	auto findChildSpawnPoint(Organism & organism, Phenome & childPhenome) -> std::optional<Util::Coord>;

    auto tryReproduce(Phenome & childPhenome, Organism & organism, i32 reproductionEnergy, i32 childEnergy) -> std::optional<Organism>;

	auto updateFactors(Settings & settings) -> void;

	auto tickFood(Settings & settings) -> void;

public:
	Environment environment;
	std::vector<Organism> organisms;

	int currentTick;

    explicit SimulationController(Environment && environment);

	auto refreshFactors(Settings & settings) -> void;
	auto tick(Settings & settings) -> void;

	auto serialize() -> json;

    auto addFood(i32 foodX, i32 foodY, Food::Type type, i32 energy) -> void;
    auto scatterFood(Food::Type type, i32 numFood, i32 energyDefault) -> void;
    auto howMuchFood() -> i32;

	auto getOrganism(UUID & id) -> Organism *;
};

#endif //ALIFE_SIMULATIONCONTROLLER_H

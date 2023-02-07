
#include "environment/simulationController.h"

SimulationController::SimulationController(Environment && environment) :
	environment(std::move(environment)),
	organisms(),
	currentStep(0) {}

auto SimulationController::step() -> void {
	//TODO put all the game loop stuff in here
	++currentStep;
}

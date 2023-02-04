#include "Environment/simulationController.h"
#include <iostream>

SimulationController::SimulationController(Environment && environment) :
	environment(std::move(environment)),
	organisms(),
	currentStep(0) {}

void SimulationController::step() {
	++currentStep;
}

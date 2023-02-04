#include "src/Environment/SimulationController.h"
#include <iostream>

SimulationController::SimulationController() : environment(environment) {
    currentStep = 0;
    environment = Environment(5);
}

void SimulationController::step() {
    environment.EnvironmentStep();
}

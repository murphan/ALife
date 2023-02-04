
#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "environment/environment.h"

#include <vector>

#include "organism.h"

class SimulationController {
public:
	Environment environment;
	std::vector<Organism> organisms;
	int currentStep;

    explicit SimulationController(Environment && environment);
    void step();
};

#endif //ALIFE_SIMULATIONCONTROLLER_H

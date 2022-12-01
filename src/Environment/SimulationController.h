#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "src/Environment/Environment.h"

class SimulationController {
    private:
        Environment environment;
        int currentStep;
    public:
        SimulationController();
        void step();

};

#endif //ALIFE_SIMULATIONCONTROLLER_H

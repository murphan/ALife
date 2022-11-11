//
// Created by Anna on 11/9/2022.
//

#ifndef ALIFE_SIMULATIONCONTROLLER_H
#define ALIFE_SIMULATIONCONTROLLER_H

#include "src/Environment/Environment.h"

namespace std {

    class SimulationController {
        public:
            SimulationController(void);
            Environment environment;
    };

} // std

#endif //ALIFE_SIMULATIONCONTROLLER_H

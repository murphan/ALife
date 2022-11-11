//
// Created by Anna on 11/9/2022.
//

#include "src/Environment/SimulationController.h"
#include <iostream>

namespace std {
    SimulationController::SimulationController() : environment(5) {
        std::cout << "Sim Controller" << endl;
    }
} // std
//
// Created by Anna on 11/9/2022.
//

#ifndef ALIFE_ENVIRONMENT_H
#define ALIFE_ENVIRONMENT_H

#include "src/Environment/MapCell.h"
#include <vector>

namespace std {


    class Environment {
        private:
            int cells;                                   //Length of side of the square map
            std::vector<std::vector<std::MapCell>> map;  //2d Vector of each cell in the map

        public:
            Environment(int);
            void EnvironmentStep();

    };

} // std

#endif //ALIFE_ENVIRONMENT_H

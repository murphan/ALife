#ifndef ALIFE_ENVIRONMENT_H
#define ALIFE_ENVIRONMENT_H

#include "src/Environment/MapCell.h"
#include <vector>

/*
// Enviorment stores the 2d vector that makes up the map and
// helps controls the values of the map cells for the organisims
// to reference.
*/

class Environment {
    private:
            int cells;                                   //Length of side of the square map
            std::vector<std::vector<std::MapCell>> map;  //2d Vector of each cell in the map

    public:
        Environment(int);
        void EnvironmentStep();

};

#endif //ALIFE_ENVIRONMENT_H

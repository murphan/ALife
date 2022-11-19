//
// Created by Anna on 11/9/2022.
//

#ifndef ALIFE_MAPCELL_H
#define ALIFE_MAPCELL_H

#include <vector>
#include "src/Environment/Food.h"

namespace std {

    class MapCell {
    private:
        int temperature;
        int light;
        std::vector<std::Food> food;
    public:
        MapCell(int inTemp, int inLight);
        void mapCellStep();
        int getTemperature();
        int getLight();

    };
}
#endif //ALIFE_MAPCELL_H

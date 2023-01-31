#ifndef ALIFE_MAPCELL_H
#define ALIFE_MAPCELL_H

#include <vector>
#include "src/Environment/Food.h"
/*
// Map Cells are the grid that cmake up the map
// they provide locations for the organisims to move through
// and contain the information that organisims need to make decisions
*/
class MapCell {
    private:
        int temperature;
        int light;
        std::vector<Food> food;
    public:
        MapCell(int parameters[4]);
        MapCell();

    void mapCellStep();
    int getTemperature();
    int getLight();

    void genFood(int Food);
};

#endif //ALIFE_MAPCELL_H
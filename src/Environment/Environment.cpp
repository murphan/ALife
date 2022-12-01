#include "Environment.h"

//Constructs a map with the given number of cells
Environment::Environment(int startCells) {
    cells = startCells;

    for (int i = 0; i < cells; ++i) {
        //creates a starting vector of vectors
        auto mapRow = std::vector<MapCell>();

        for (int j = 0; j < cells; ++j) {
            //creates mapCells and adds them to the row
            auto col = MapCell(10, 10);
            mapRow.emplace_back(col);
        }
        //add the row to the map
        map.emplace_back((mapRow));
    }
}

//Preforms actions for each step of the program
void Environment::EnvironmentStep() {
    //Moves through the map and updates each cell
    for (int i = 0; i < cells; ++i) {

        for (int j = 0; j < cells; ++j) {
            map[i][j].mapCellStep();
        }
    }

}
#include "Environment.h"

//Constructs a map with the given number of cells
Environment::Environment(int startCells) {
    cells = startCells;
    //creates a starting vector of vectors
    map.reserve(cells*cells);
    int parameters[4] = {5, 5, 0, 0};

    for (int i = 0; i < cells*cells; ++i) {
        //creates mapCells and adds them to the vector
        auto newCell = MapCell(parameters);
        map.emplace_back(newCell);
    }
}

//Preforms actions for each step of the program
void Environment::EnvironmentStep() {
    //Moves through the map and updates each cell
    for (int i = 0; i < cells*cells; ++i) {
            map[i].mapCellStep();
    }

}

MapCell Environment::getCell(int x, int y) {
    return map[x+y*x];
}

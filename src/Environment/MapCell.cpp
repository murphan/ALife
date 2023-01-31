#include "src/Environment/MapCell.h"

/*
//MapCell constructor: takes starting temperature and light level
//                     and initializes food array
*/
MapCell::MapCell(int params[4]) :
                                                temperature(params[0]),
                                                light(params[1]) {

    genFood(10);
}

MapCell::MapCell() : temperature(0), light(0) {
    genFood(0);
}

void MapCell::genFood(int foodNum) {
    if (foodNum > 0) {
        food.reserve(foodNum);
        for (int i = 0; i < foodNum; ++i) {
            food.emplace_back(100, .9, 0);
        }
    }
}
//Map step: cycles through all food and calls their step
//          functions.
void MapCell::mapCellStep(){

    int numFood = food.size();
    if (numFood == 0)
        return ;

    for (int i = 0; i < numFood; ++i) {
        food[i].foodStep();
    }

    /*
     * Reserved space for altering light and temp levels
     */
}

//Returns current temperature
int MapCell::getTemperature(){
    return temperature;
}

//Returns current light level
int MapCell::getLight(){
    return light;
}


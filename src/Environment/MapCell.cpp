//
// Created by Anna on 11/9/2022.
//

#include "src/Environment/MapCell.h"
#include "iostream"

namespace std {

    //MapCell constructor: takes starting temperature and light level
    //                     and initializes food array
    MapCell::MapCell(int newTemp, int newLight) :
                                                    temperature(newTemp),
                                                    light(newLight) {

        food.reserve(10);
        for (int i = 0; i < 10; ++i) {
            food.emplace_back(100, .9, 0);
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

} // std
//
// Created by Anna on 11/10/2022.
//

#include "src/Environment/Food.h"
#include "iostream"

namespace std {

    //Food constructor: Takes starting energy, decay rate, and an int representing type
    Food::Food(int startingEnergy, float startingDecay, int inType) :
                                                            energy(startingEnergy),
                                                            decay(startingDecay),
                                                            type(inType) {}

    //foodStep: preforms actions for a step of the program
    // such as reduce energy in the food by decay rate
    void Food::foodStep() {
        energy*=decay;
    }

    //returns current energy
    int Food::getEnergy() {
        return energy;
    }

    //returns Type
    //**NOTE: This is a base implementation of the system with only one type.**
    int Food::getType(){
        return type;
    }


} // std